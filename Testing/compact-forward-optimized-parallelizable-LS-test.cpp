#include <bits/stdc++.h>
#include "ECLgraph.h"
using namespace std;

int main(int argc, char *argv[])
{ // Linear scan + CSR

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
    // Counting the triangles. Fully parallelizable with no race conditions.
    int count = 0;
    int u, v, w;
    int L, R;
    for (int i = 0; i < V; i += 1)
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

    cout << "The number of nodes in the graph = " << g.nodes << ".\n";
    cout << "The number of triangles present = " << count << ".\n";
    freeECLgraph(g);
    return 0;
}