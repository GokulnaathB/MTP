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

    vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}, {5, 6}, {5, 7}, {5, 8}, {6, 7}, {6, 8}, {7, 8}, {0, 5}, {1, 6}, {2, 7}, {3, 8}};
    // Has got 14 triangles.

    // vector<pair<int, int>> edges = {
    //     {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 4}, {3, 5}, {4, 5}, {6, 7}, {6, 8}, {6, 9}, {6, 10}, {7, 8}, {7, 9}, {7, 10}, {8, 9}, {8, 10}, {9, 10}, {11, 12}, {11, 13}, {11, 14}, {12, 13}, {12, 14}, {13, 14}};
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
    // Counting the triangles. Fully parallelizable with no race conditions.
    count = 0;
    int w;
    int L, R;
    for (int i = 0; i < x + 1; i += 1)
    {
        u = i;
        l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        for (int j = l; j <= r; j += 1) // O(total_number_of_edges) across all the iterations of the outer loop.
        {
            v = fwd_neighbors[j];
            L = f_ptrs[v], R = f_ptrs[v + 1] - 1;
            for (int k = L; k <= R; k += 1) // O(maximum_degree)
            {
                w = fwd_neighbors[k];
                for (int p = l; p <= r; p += 1) // O(degree_of_u) <= O(maximum_degree)
                    if (fwd_neighbors[p] == w)
                    {
                        count += 1;
                        break;
                    }
            }
        }
    } // TC = O(total_number_of_edges x delta^2).
    /* For dense graphs, delta could nearly equal the total number of nodes, in which case O(total_number_of_edges x total_number_of_vertices) is better than this approach. This approach could be better (can't say for sure) for sparse graphs and most of real-world graphs are sparse!
    There's no free lunch here — it's always a tradeoff between space, time, and parallelizability.
    */
    /*
    Space Complexity:
    For fwd_neighbors: Each undirected edge (u,v) contributes exactly one entry — either v goes into u's list (if u < v) or u goes into v's list (if v < u). So, O(total_number_of_edges) = O(m).
    */

    // for (auto v : values)
    //     cout << v << " ";
    // cout << "\n";
    // for (auto ptr : ptrs)
    //     cout << ptr << " ";
    // cout << "\n";

    cout << "The number of triangles present = " << count << ".\n";
    return 0;
}