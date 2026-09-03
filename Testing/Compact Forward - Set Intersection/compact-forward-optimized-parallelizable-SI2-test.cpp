#include <bits/stdc++.h>
#include <chrono>
#include "../ECLgraph.h"
using namespace std;

int l, r, mid;
int x, y;
void binarySearchIntersection(int &u, int &v, vector<int> &f_ptrs, vector<int> &fwd_neighbors, int &count)
{
    for (int j = f_ptrs[v]; j < f_ptrs[v + 1]; j += 1)
    {
        x = fwd_neighbors[j];
        l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        while (l <= r)
        {
            mid = l + (r - l) / 2;
            if (fwd_neighbors[mid] == x)
            {
                count += 1;
                break;
            }
            else if (fwd_neighbors[mid] > x)
                r = mid - 1;
            else
                l = mid + 1;
        }
    }
}

void mergeBasedIntersection(int &u, int &v, vector<int> &f_ptrs, vector<int> &fwd_neighbors, int &count)
{
    x = f_ptrs[u], y = f_ptrs[v];
    while (x < f_ptrs[u + 1] and y < f_ptrs[v + 1])
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
    int u, v;
    int size_fwdN_u, size_fwdN_v;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < V; i += 1)
    {
        u = i;
        size_fwdN_u = f_ptrs[u + 1] - f_ptrs[u];

        for (int j = f_ptrs[u]; j < f_ptrs[u + 1]; j += 1) // O(total_number_of_edges)
        {
            v = fwd_neighbors[j];
            size_fwdN_v = f_ptrs[v + 1] - f_ptrs[v];

            if (!size_fwdN_u or !size_fwdN_v)
                continue;

            if (size_fwdN_u > size_fwdN_v)
            {
                if (size_fwdN_v * log2(size_fwdN_u) < size_fwdN_u + size_fwdN_v)
                {
                    // Binary search intersection
                    binarySearchIntersection(u, v, f_ptrs, fwd_neighbors, count);
                }
                else
                {
                    // Merge based intersection
                    mergeBasedIntersection(u, v, f_ptrs, fwd_neighbors, count);
                }
            }
            else
            {
                if (size_fwdN_u * log2(size_fwdN_v) < size_fwdN_u + size_fwdN_v)
                {
                    // Binary search intersection
                    binarySearchIntersection(v, u, f_ptrs, fwd_neighbors, count);
                }
                else
                {
                    // Merge based intersection
                    mergeBasedIntersection(u, v, f_ptrs, fwd_neighbors, count);
                }
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "The number of nodes in the graph = " << g.nodes << ".\n";
    cout << "The number of triangles present = " << count << ".\n";
    cout << "Time taken: " << elapsed.count() << " seconds\n";
    freeECLgraph(g);
    return 0;
}