#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int main()
{

    // #############################################
    vector<pair<int, int>> edges;
    // Random graph generation
    int n = 500, m = 50000;
    srand(42);
    for (int i = 0; i < m; i++)
    {
        int u = rand() % n;
        int v = rand() % n;
        if (u != v)
            edges.push_back({u, v});
    }

    // #############################################
    // Finding the vertex with the highest value.
    int V = INT_MIN;
    for (auto edge : edges)
    {
        V = max(V, edge.first);
        V = max(V, edge.second);
    }

    // #############################################
    // Creating the adjacency matrix.
    vector<vector<int>> adjM(V + 1);
    for (int i = 0; i < V + 1; i += 1)
        for (int j = 0; j < V + 1; j += 1)
            adjM[i].push_back(0);
    for (auto edge : edges)
    {
        adjM[edge.first][edge.second] = 1;
        adjM[edge.second][edge.first] = 1;
    }

    // #############################################
    // Calculating the number of triangles in the graph.
    int count = 0;
    double start = omp_get_wtime();
#pragma omp parallel for reduction(+ : count) schedule(runtime)
    for (int i = 0; i < V + 1; i += 1)
    {
        for (int j = 0; j < V + 1; j += 1)
        {
            if (i == j)
                continue;
            for (int k = 0; k < V + 1; k += 1)
            {
                if (i == k or j == k)
                    continue;
                if (adjM[i][j] and adjM[j][k] and adjM[i][k])
                    count += 1;
            }
        }
    } // TC: O(V^3), SC: O(V^2)
    count /= 6;
    cout << "Parallel time: " << omp_get_wtime() - start << " seconds.\n";
    cout << "Parallel: The number of triangles in the given graph is " << count << ".\n";
    cout << "\n\n";

    start = omp_get_wtime();
    count = 0;
    for (int i = 0; i < V + 1; i += 1)
    {
        for (int j = 0; j < V + 1; j += 1)
        {
            if (i == j)
                continue;
            for (int k = 0; k < V + 1; k += 1)
            {
                if (i == k or j == k)
                    continue;
                if (adjM[i][j] and adjM[j][k] and adjM[i][k])
                    count += 1;
            }
        }
    } // TC: O(V^3), SC: O(V^2)
    count /= 6;
    cout << "Sequential time: " << omp_get_wtime() - start << " seconds.\n";
    cout << "Sequential: The number of triangles in the given graph is " << count << ".\n";

    return 0;
}