#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
#include "../ECLgraph.h"
using namespace std;

void binarySearchIntersection(int &u, int &v, vector<int> &f_ptrs, vector<int> &fwd_neighbors, long long &count)
{
    for (int j = f_ptrs[v]; j < f_ptrs[v + 1]; j += 1)
    {
        int x = fwd_neighbors[j];
        int l = f_ptrs[u], r = f_ptrs[u + 1] - 1;
        while (l <= r)
        {
            int mid = l + (r - l) / 2;
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

void mergeBasedIntersection(int &u, int &v, vector<int> &f_ptrs, vector<int> &fwd_neighbors, long long &count)
{
    int x = f_ptrs[u], y = f_ptrs[v];
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
    auto total_start = chrono::high_resolution_clock::now();
    int V = g.nodes;

    vector<int> degree(V, 0);
    int max_degree = 0;
#pragma omp parallel for reduction(max : max_degree)
    for (int i = 0; i < V; i += 1)
    {
        int l, r;
        l = g.nindex[i], r = g.nindex[i + 1];
        degree[i] = r - l;
        if (degree[i] > max_degree)
            max_degree = degree[i];
    }

    vector<int> freq(max_degree + 1, 0);
#pragma omp parallel for
    for (int i = 0; i < V; i += 1)
    {
        int d = degree[i];

#pragma omp atomic
        freq[d] += 1;
    }

    // BEFORE:
    vector<int> prefix_sum(max_degree + 1, 0);
    prefix_sum[0] = freq[0];
    for (int i = 1; i <= max_degree; i += 1)
        prefix_sum[i] = freq[i] + prefix_sum[i - 1];

    // AFTER:
    //     int n = freq.size();
    //     int num_threads = omp_get_max_threads();
    //     vector<int> prefix_sum(max_degree + 1, 0);
    //     vector<int> thread_sum(num_threads, 0);
    //     vector<int> offset(num_threads);
    // #pragma omp parallel
    //     {
    //         int tid = omp_get_thread_num();

    //         int start = tid * n / num_threads;
    //         int end = (tid + 1) * n / num_threads;

    //         if (start < end)
    //         {
    //             // Step 1: Local Prefix Sum
    //             prefix_sum[start] = freq[start];

    //             for (int i = start + 1; i < end; i += 1)
    //                 prefix_sum[i] = prefix_sum[i - 1] + freq[i];

    //             thread_sum[tid] = prefix_sum[end - 1]; // Storing this thread's local prefix sum.
    //         }

    // #pragma omp barrier // We need this barrier because, for example, offset of thread 2 depends on the local prefix sums of thread 1 and thread 2. So, thread 2 can't go ahead to calculate its offset if thread 0 and thread 1 haven't calulated their local sums yet.

    //         // Every thread must finish its local prefix sum before any thread calculates its offset, because offset[tid] depends on the thread_sum values of all preceding threads.

    //         // Step 2: Calculate offset.
    //         offset[tid] = 0;
    //         for (int i = 0; i < tid; i += 1)
    //             offset[tid] += thread_sum[i];

    //         // Step 3: Add offset
    //         for (int i = start; i < end; i += 1)
    //             prefix_sum[i] += offset[tid];
    //     }
    // Not efficient. For just reading two integers and writing an integer, we're performing three phases along with a barrier. That's more coordination to perform a very small amount of arithmetic.
    // With a tiny workload, the overhead dominates the entire operation. Here the size of freq is max_degree, which might be much smaller than V. So the prefix sum was over a relatively small array. Parallelizing it added overhead without enough work.
    // With a large workload, the overhead would be small compared with the computation you're speeding up.

    vector<int> old_to_new(V, 0);
    vector<int> new_to_old(V, 0);
    for (int i = 0; i < V; i += 1)
    {
        int old_node = i;
        int deg = degree[old_node];
        int new_node = --prefix_sum[deg]; // todo here.
        old_to_new[old_node] = new_node;
        new_to_old[new_node] = old_node;
    }

    // Construct fwd_neighbors and f_ptrs
    vector<int> fwd_neighbors;
    vector<int> f_ptrs(V + 1, 0);
    for (int i = 0; i < V; i += 1)
    {
        f_ptrs[i] = fwd_neighbors.size(); // try using parallel scan.
        // find the number of fwd_neighbors (thread 0 calculates the number of fwd_neighbors of vertex 0)
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

    // AFTER:
//     vector<int> fwd_degree(V, 0);
// #pragma omp parallel for
//     for (int new_node = 0; new_node < V; new_node += 1)
//     {
//         int old_node = new_to_old[new_node];
//         int l = g.nindex[old_node], r = g.nindex[old_node + 1];

//         int count = 0;

//         for (int j = l; j < r; j += 1)
//         {
//             int old_neighbor = g.nlist[j];
//             int new_neighbor = old_to_new[old_neighbor];
//             if (new_node < new_neighbor)
//                 count += 1;
//         }

//         fwd_degree[new_node] = count;
//     }

//     vector<int> f_ptrs(V + 1, 0);
//     for (int i = 1; i <= V; i += 1)
//         f_ptrs[i] = fwd_degree[i - 1] + f_ptrs[i - 1];

//     vector<int> fwd_neighbors(f_ptrs[V]);
// #pragma omp parallel for
//     for (int new_node = 0; new_node < V; new_node += 1)
//     {
//         int old_node = new_to_old[new_node];
//         int l = g.nindex[old_node], r = g.nindex[old_node + 1];

//         int pos = f_ptrs[new_node];

//         for (int j = l; j < r; j += 1)
//         {
//             int old_neighbor = g.nlist[j];
//             int new_neighbor = old_to_new[old_neighbor];
//             if (new_node < new_neighbor)
//             {
//                 fwd_neighbors[pos] = new_neighbor;
//                 pos += 1;
//             }
//         }
//     }

// Sorting each node's forward neigbors
#pragma omp parallel for schedule(dynamic, 64)
    for (int i = 0; i < V; i += 1)
    {
        int l = f_ptrs[i], r = f_ptrs[i + 1];
        sort(fwd_neighbors.begin() + l, fwd_neighbors.begin() + r); // parallel sorting algorithm for large arrays.
    }

    vector<int> edgeSource(fwd_neighbors.size(), 0);
    for (int u = 0; u < V; u += 1)
    {
        for (int j = f_ptrs[u]; j < f_ptrs[u + 1]; j += 1)
            edgeSource[j] = u;
    }

    auto preprocessing_end = chrono::high_resolution_clock::now();

    // Counting
    long long count = 0;
    auto start = chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(dynamic, 64) reduction(+ : count)
    for (int i = 0; i < fwd_neighbors.size(); i += 1)
    {
        int u = edgeSource[i];
        int v = fwd_neighbors[i];
        int size_fwdN_u = f_ptrs[u + 1] - f_ptrs[u];
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
    auto end = chrono::high_resolution_clock::now();
    // Edge-level parallelism provides finer-grained load balancing, which is particularly beneficial for larger and/or highly skewed graphs, but its overhead may outweigh its benefits for smaller workloads.

    // Counting
    //     auto start = chrono::high_resolution_clock::now();
    //     long long count = 0;
    // #pragma omp parallel for schedule(dynamic, 64) reduction(+ : count)
    //     for (int i = 0; i < V; i += 1)
    //     {
    //         int u = i;
    //         int size_fwdN_u = f_ptrs[u + 1] - f_ptrs[u];
    //         int l = f_ptrs[u], r = f_ptrs[u + 1];
    //         for (int j = l; j < r; j += 1)
    //         {
    //             int v = fwd_neighbors[j];
    //             int size_fwdN_v = f_ptrs[v + 1] - f_ptrs[v];

    //             if (!size_fwdN_u or !size_fwdN_v)
    //                 continue;
    //             if (size_fwdN_u > size_fwdN_v)
    //             {
    //                 if (size_fwdN_v * log2(size_fwdN_u) < size_fwdN_u + size_fwdN_v)
    //                 {
    //                     // Binary search intersection
    //                     binarySearchIntersection(u, v, f_ptrs, fwd_neighbors, count);
    //                 }
    //                 else
    //                 {
    //                     // Merge based intersection
    //                     mergeBasedIntersection(u, v, f_ptrs, fwd_neighbors, count);
    //                 }
    //             }
    //             else
    //             {
    //                 if (size_fwdN_u * log2(size_fwdN_v) < size_fwdN_u + size_fwdN_v)
    //                 {
    //                     // Binary search intersection
    //                     binarySearchIntersection(v, u, f_ptrs, fwd_neighbors, count);
    //                 }
    //                 else
    //                 {
    //                     // Merge based intersection
    //                     mergeBasedIntersection(u, v, f_ptrs, fwd_neighbors, count);
    //                 }
    //             }
    //         }
    //     }

    //     auto end = chrono::high_resolution_clock::now();
    auto total_end = chrono::high_resolution_clock::now();

    chrono::duration<double> preprocessing_time =
        preprocessing_end - total_start;

    chrono::duration<double> counting_time =
        end - start;

    chrono::duration<double> total_time =
        total_end - total_start;

    cout << "The number of nodes in the graph = " << g.nodes << ".\n";
    cout << "The number of triangles present = " << count << ".\n";

    cout << "Preprocessing time: "
         << preprocessing_time.count() << " seconds\n";

    cout << "Counting time: "
         << counting_time.count() << " seconds\n";

    cout << "Total time: "
         << total_time.count() << " seconds\n";
    cout << "Size of fwd_neighbors " << fwd_neighbors.size() << "\n";
    freeECLgraph(g);
    return 0;
}