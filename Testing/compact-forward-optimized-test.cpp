#include <bits/stdc++.h>
#include "ECLgraph.h"
using namespace std;

int main(int argc, char *argv[])
{

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
    // Counting the triangles (not parallelizable).
    int count = 0;
    vector<int> visited(V, 0);
    int u, v, w;
    int L, R;
    for (int i = 0; i <= V - 1; i += 1)
    {
        u = i;
        l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        for (int j = l; j <= r; j += 1)
            visited[fwd_neighbors[j]] = 1;

        for (int j = l; j <= r; j += 1)
        {
            v = fwd_neighbors[j];
            L = f_ptrs[v], R = f_ptrs[v + 1] - 1;
            for (int k = L; k <= R; k += 1)
            {
                w = fwd_neighbors[k];
                if (visited[w])
                    count += 1;
            }
        }

        for (int j = l; j <= r; j += 1)
            visited[fwd_neighbors[j]] = 0;
    }
    /* Why isn't this approach parallelizable?
    Consider two nodes u1 and u2 being processed by different threads simultaneously. u1's forward neighbor happens to be node 5, and u2's forward neighbor also happens to be node 5. Both of these threads mark 5 as visitied and proceed to count triangles.
    If thread 1 completes soon and unmarks node 5 while Thread 2 is still counting, thread 2 may miss triangles. Here's a race condition: it isn't in the marking, it's in the unmarking! One thread's unmark corrupts another thread's active computation.
    */

    cout << "The number of nodes in the graph = " << g.nodes << ".\n";
    cout << "The number of triangles present = " << count << ".\n";
    freeECLgraph(g);
    return 0;
}