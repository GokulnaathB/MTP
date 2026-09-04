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

    vector<int> degree(V, 0);
    int max_degree = 0;
    for (int i = 0; i < V; i += 1)
    {
        int l, r;
        l = g.nindex[i], r = g.nindex[i + 1];
        degree[i] = r - l;
        if (degree[i] > max_degree)
            max_degree = degree[i];
    }

    vector<int> freq(max_degree + 1, 0);
    for (auto &d : degree)
        freq[d] += 1;

    vector<int> prefix_sum(max_degree + 1, 0);
    prefix_sum[0] = freq[0];
    for (int i = 1; i <= max_degree; i += 1)
        prefix_sum[i] = freq[i] + prefix_sum[i - 1];

    vector<int> old_to_new(V, 0);
    vector<int> new_to_old(V, 0);
    for (int i = 0; i < V; i += 1)
    {
        int old_node = i;
        int deg = degree[old_node];
        int new_node = --prefix_sum[deg];
        old_to_new[old_node] = new_node;
        new_to_old[new_node] = old_node;
    }

    // Construct fwd_neighbors and f_ptrs
    vector<int> fwd_neighbors;
    vector<int> f_ptrs(V + 1, 0);
    for (int i = 0; i < V; i += 1)
    {
        f_ptrs[i] = fwd_neighbors.size();

        int new_node = i;
        int old_node = new_to_old[new_node];
        int l = g.nindex[old_node], r = g.nindex[old_node + 1];
        for (int j = l; j < r; j += 1)
        {
            int old_neighbor = g.nlist[j];
            int new_neighbor = old_to_new[old_neighbor];
            if (new_node < new_neighbor)
                fwd_neighbors.push_back(new_neighbor);
        }
    }
    f_ptrs[V] = fwd_neighbors.size();

    // Sorting each node's forward neigbors
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < V; i += 1)
    {
        int l = f_ptrs[i], r = f_ptrs[i + 1];
        sort(fwd_neighbors.begin() + l, fwd_neighbors.begin() + r);
    }

    // Counting
    int count = 0;
    for (int i = 0; i < V; i += 1)
    {
        int u = i;
        int size_fwdN_u = f_ptrs[u + 1] - f_ptrs[u];
        int l = f_ptrs[u], r = f_ptrs[u + 1];
        for (int j = l; j < r; j += 1)
        {
            int v = fwd_neighbors[j];
            int size_fwdN_v = f_ptrs[v + 1] - f_ptrs[v];

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
}