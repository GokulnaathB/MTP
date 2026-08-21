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

    // 1. Calculate the maximum number of bits required for each node, which should be a multiple of 64.
    int max_fwd_neighbor, SIZE, n;
    vector<vector<uint64_t>> fwd_bits(V);
    for (int i = 0; i < V; i += 1)
    {
        max_fwd_neighbor = INT_MIN;
        l = f_ptrs[i], r = f_ptrs[i + 1] - 1;
        for (int j = l; j <= r; j += 1)
            max_fwd_neighbor = max(max_fwd_neighbor, fwd_neighbors[j]);

        if (max_fwd_neighbor != INT_MIN)
        {
            n = max_fwd_neighbor + 1;
            SIZE = (n + 63) / 64;
            // Now have to insert SIZE uint64_t zeros in fwd_bits[i].
            for (int j = 0; j < SIZE; j += 1)
                fwd_bits[i].push_back(0);
        }
    }

    // 2. Setting the bits of fwd_bits[i] corresponding to its forward neighbors.
    int u, v;
    for (int i = 0; i < V; i += 1)
    {
        u = i;
        l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        for (int j = l; j <= r; j += 1)
        {
            v = fwd_neighbors[j];
            fwd_bits[u][v / 64] |= (1ULL << (v % 64));
        }
    }

    // #############################################
    // Counting the triangles. Fully parallelizable with no race conditions.
    int count = 0, s1, s2;
    for (int i = 0; i < V; i += 1)
    {
        u = i;
        l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        for (int j = l; j <= r; j += 1)
        {
            v = fwd_neighbors[j];
            s1 = fwd_bits[u].size(), s2 = fwd_bits[v].size();
            for (int k = 0; k < min(s1, s2); k += 1)
                count += __builtin_popcountll(fwd_bits[u][k] & fwd_bits[v][k]);
        }
    }
    // This version has drastically better space complexity in practice, though their theoretical worst-case Big-O remains the same.

    cout << "The number of nodes in the graph = " << g.nodes << ".\n";
    cout << "The number of triangles present = " << count << ".\n";
    freeECLgraph(g);

    return 0;
}