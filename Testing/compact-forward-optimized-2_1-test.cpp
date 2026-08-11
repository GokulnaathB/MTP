#include <bits/stdc++.h>
#include "ECLgraph.h"
using namespace std;

int main(int argc, char *argv[])
{ // fwd_bits + CSR

    // #############################################
    // Creating an ECLgraph object of the input graph.
    ECLgraph g = readECLgraph(argv[1]);
    int V = g.nodes;

    // #############################################
    // Constructing the flat forward neighbors array.
    vector<int> fwd_neighbors;
    vector<int> f_ptrs(V + 1, 0);
    int l, r;
    for (int i = 0; i <= V - 1; i += 1)
    {
        f_ptrs[i] = fwd_neighbors.size();
        l = g.nindex[i], r = g.nindex[i + 1] - 1;
        for (int j = l; j <= r; j += 1)
            if (g.nlist[j] > i)
                fwd_neighbors.push_back(g.nlist[j]);
    }
    f_ptrs[V] = fwd_neighbors.size();

    // #############################################
    // Constructing the forward bits matrix.
    // There are x+1 nodes.
    int num_of_64s = (V + 63) / 64;
    vector<vector<uint64_t>> fwd_bits(V, vector<uint64_t>(num_of_64s, 0));
    /*
    Why uint64_t?
    We want each element to hold exactly 64 bits. Type uint64_t is unsigned 64 bits — no sign bit, all 64 bits are usable for storing neighbor information. Safe for all bitwise operations.
    */
    int u, v;
    for (int i = 0; i <= V - 1; i += 1)
    {
        u = i;
        l = f_ptrs[i], r = f_ptrs[i + 1] - 1;
        for (int j = l; j <= r; j += 1)
        {
            v = fwd_neighbors[j];
            fwd_bits[u][v / 64] |= (1ULL << (v % 64));
            /*
            Why not 1 << (v % 64)?
            v % 64 can be anywhere from 0 to 63 — so if v % 64 >= 31, you're shifting a 32-bit signed int into (sign bit changes) or past the sign bit (doesn't fit in 32 bits) → undefined behavior.
            1ULL is a 64-bit unsigned int — shifting up to 63 positions is always safe, no sign bit to worry about.
            That's why 1ULL is used specifically — not because 1 is negative, but because it's only 32 bits and shifts beyond 30 are dangerous!
            (The size of 1 depends on its type, not the system architecture. 1 is always an int in C++, and int is always 32 bits on most systems regardless of whether the system is 32-bit or 64-bit. The 64-bit in "64-bit system" refers to the size of pointers and memory addresses, not the size of int.)
            */
        }
    }
    /*
    uint64_t for the vector and 1ULL for left shift — they're consistent with each other. If you used 1 instead of 1ULL, you'd be shifting a 32-bit value and OR-ing it into a 64-bit value — inconsistent and potentially undefined behavior.
    */
    /*
    Why choose uint64_t in the first place?
    Because most CPUs natively support 64-bit operations in hardware.
    uint64_t AND, OR, POPCNT → single CPU instruction.
    uint128_t No native hardware support on most CPUs.
    */

    // #############################################
    // Counting the triangles. Fully parallelizable with no race conditions.
    // uint64_t holder;
    int count = 0;
    for (int i = 0; i <= V - 1; i += 1)
    {
        u = i;
        l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        for (int j = l; j <= r; j += 1) // O(2*total_number_edges) = O(total_number_of_edges) across all the iterations of the outer loop.
        {
            v = fwd_neighbors[j];
            for (int k = 0; k < num_of_64s; k += 1) // O(ceil(n/64)) = O(n)
            {
                // Method 1
                /*
                holder = fwd_bits[u][k] & fwd_bits[v][k];
                for (int m = 63; m >= 0; m -= 1)
                    count += (holder >> m) & 1;
                */

                // Method 2
                count += __builtin_popcountll(fwd_bits[u][k] & fwd_bits[v][k]); // each iteration is processing 64 nodes simultaneously, hence 64x practical speedup over checking one node at a time.
                /* A modern CPU can perform operations on 64 bits in a single instruction.
                __builtin_popcountll is a single CPU instruction (POPCNT) on modern processors.
                The AND operation is also a single instruction. So the entire line is O(1). */
            }
        }
    } // TC = O(total_number_of_edges x total_number_of_vertices).
    /*
    Method 1 vs Method 2:
    A single CPU instruction vs 64 iterations of a loop. The constant matters enormously here — that's a 64x difference in actual execution time even though both are O(1)! This is one of those cases where Big-O notation doesn't tell the full story!
    Counting all the bits in one CPU cycle — that's the whole speedup!
    __builtin_popcountll maps directly to the POPCNT instruction that exists in modern Intel/AMD CPUs. The hardware literally has dedicated circuitry to count bits in a 64-bit number in one clock cycle.
    */
    /*
    Space Complexity:
    For fwd_bits: V x V/64 x 64bits = V x V/64 x 8 bytes = V x V/8 bytes = O(V^2 / 8).
    */

    cout << "The number of nodes in the graph = " << g.nodes << ".\n";
    cout << "The number of triangles present = " << count << ".\n";
    freeECLgraph(g);
    return 0;
}