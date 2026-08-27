#include <bits/stdc++.h>
#include "../ECLgraph.h"
using namespace std;

int main(int argc, char *argv[])
{ // Set Intersection + CSR

    // #############################################
    // Creating an ECLgraph object of the input graph.
    ECLgraph g = readECLgraph(argv[1]);
    int V = g.nodes;

    // #############################################
    // Constructing the flat forward neighbors array.
    vector<int> fwd_neighbors; // SC = O(total_number_of_edges)
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
    // Counting the triangles. Fully parallelizable with no race conditions (only reads taking place).
    int count = 0;
    int u, v, w;
    int L, R;
    int x, y;
    for (int i = 0; i < V; i += 1)
    {
        u = i;
        l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        for (int j = l; j <= r; j += 1) // O(total_number_of_edges)
        {
            v = fwd_neighbors[j];

            x = f_ptrs[u], y = f_ptrs[v];
            while (x < f_ptrs[u + 1] and y < f_ptrs[v + 1]) // O(maximum_degree)
            {
                if (fwd_neighbors[x] < fwd_neighbors[y])
                    x += 1;
                else if (fwd_neighbors[x] > fwd_neighbors[y])
                    y += 1;
                else
                {
                    count += 1;
                    x += 1, y += 1;
                }
            }
        }
    } // TC = O(total_number_of_edes x delta)

    cout << "The number of nodes in the graph = " << g.nodes << ".\n";
    cout << "The number of triangles present = " << count << ".\n";
    freeECLgraph(g);
    return 0;
}