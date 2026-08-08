#include <bits/stdc++.h>
using namespace std;

int main()
{

    // #############################################
    // Taking graph edges input from the user.
    int x, y, z;
    int count = 1;
    // vector<pair<int, int>> edges;
    // cout << "ENTER THE EDGES OF THE GRAPH\n\n";
    // while (true)
    // {
    //     cout << "Enter the starting vertex of edge " << count << ": ";
    //     cin >> x;
    //     cout << "Enter the ending vertex of edge " << count << ": ";
    //     cin >> y;
    //     cout << "\n";
    //     edges.push_back({x, y});
    //     cout << "Enter 1 to continue or 0 to quit: ";
    //     cin >> z;
    //     if (z == 1)
    //     {
    //         cout << "\n";
    //         count += 1;
    //         continue;
    //     }
    //     if (z == 0)
    //     {
    //         cout << "\n";
    //         break;
    //     }
    //     while (z)
    //     {
    //         cout << "Enter only either 1 or 0, 1 to continue and 0 to quit: ";
    //         cin >> z;
    //         if (z == 1)
    //         {
    //             cout << "\n";
    //             count += 1;
    //             break;
    //         }
    //         if (z == 0)
    //         {
    //             cout << "\n";
    //             break;
    //         }
    //     }
    //     if (!z)
    //         break;
    // }

    // vector<pair<int, int>> edges = {
    //     {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}, {5, 6}, {5, 7}, {5, 8}, {6, 7}, {6, 8}, {7, 8}, {0, 5}, {1, 6}, {2, 7}, {3, 8}};
    // Has got 14 triangles.

    vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 4}, {3, 5}, {4, 5}, {6, 7}, {6, 8}, {6, 9}, {6, 10}, {7, 8}, {7, 9}, {7, 10}, {8, 9}, {8, 10}, {9, 10}, {11, 12}, {11, 13}, {11, 14}, {12, 13}, {12, 14}, {13, 14}};
    // Has got 34 triangles.

    // #############################################
    // Finding the highest node value.
    x = INT_MIN;
    for (auto edge : edges)
    {
        x = max(x, edge.first);
        x = max(x, edge.second);
    }

    // #############################################
    // Finding how many neighbors each node has got.
    vector<int> neighbor_count(x + 1, 0);
    for (auto edge : edges)
    {
        neighbor_count[edge.first] += 1;
        neighbor_count[edge.second] += 1;
    }

    // #############################################
    // Finding what the size of values array should be.
    int s = 0;
    for (auto count : neighbor_count)
        s += count;

    // #############################################
    // Declaring and initializing data structures to keep track of the next index where a neighbor of a node must be placed in values.
    vector<int> node_idx(x + 1, 0);
    int prev_ending = neighbor_count[0] - 1;
    node_idx[0] = 0;
    for (int i = 1; i <= x; i += 1)
    {
        node_idx[i] = prev_ending + 1;
        prev_ending = prev_ending + neighbor_count[i];
    }

    vector<int> ptrs(x + 2, 0);
    for (int i = 0; i <= x; i += 1)
        ptrs[i] = node_idx[i];
    ptrs[x + 1] = s;

    // #############################################
    // CSR-Representing the graph.
    vector<int> values(s, 0);
    int u, v, idx_u, idx_v;
    for (auto edge : edges)
    {
        u = edge.first, v = edge.second;
        idx_u = node_idx[u], idx_v = node_idx[v];
        values[idx_u] = v, values[idx_v] = u;
        node_idx[u] += 1, node_idx[v] += 1;
    }

    // #############################################
    // Constructing the flat forward neighbors array.
    vector<int> fwd_neighbors;
    vector<int> f_ptrs(x + 2, 0);
    int l, r;
    for (int i = 0; i <= x; i += 1)
    {
        f_ptrs[i] = fwd_neighbors.size();
        l = ptrs[i], r = ptrs[i + 1] - 1;
        for (int j = l; j <= r; j += 1)
            if (values[j] > i)
                fwd_neighbors.push_back(values[j]);
    }
    f_ptrs[x + 1] = fwd_neighbors.size();

    // #############################################
    // Constructing the forward bits matrix.
    // There are x+1 nodes.
    int num_of_64s = ((x + 1) + 63) / 64;
    vector<vector<uint64_t>> fwd_bits(x + 1, vector<uint64_t>(num_of_64s, 0));

    for (int i = 0; i <= x; i += 1)
    {
        u = i;
        l = f_ptrs[i], r = f_ptrs[i + 1] - 1;
        for (int j = l; j <= r; j += 1)
        {
            v = fwd_neighbors[j];
            fwd_bits[u][v / 64] |= (1ULL << (v % 64));
        }
    }

    // #############################################
    // Counting the triangles. Fully parallelizable with no race conditions.
    uint64_t holder;
    count = 0;
    for (int i = 0; i <= x; i += 1)
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

    cout << "The number of triangles present = " << count << ".\n";
    return 0;
}