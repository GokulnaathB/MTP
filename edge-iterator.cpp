#include <bits/stdc++.h>
using namespace std;

int main()
{
    // #############################################
    // Taking graph edges input from the user.
    vector<pair<int, int>> edges;
    cout << "ENTER THE EDGES OF THE GRAPH\n\n";
    int x, y, z;
    int count = 1;
    while (true)
    {
        cout << "Enter the starting vertex of edge " << count << ": ";
        cin >> x;
        cout << "Enter the ending vertex of edge " << count << ": ";
        cin >> y;
        cout << "\n";
        edges.push_back({x, y});
        cout << "Enter 1 to continue or 0 to quit: ";
        cin >> z;
        if (z == 1)
        {
            cout << "\n";
            count += 1;
            continue;
        }
        if (z == 0)
        {
            cout << "\n";
            break;
        }
        while (z)
        {
            cout << "Enter only either 1 or 0, 1 to continue and 0 to quit: ";
            cin >> z;
            if (z == 1)
            {
                cout << "\n";
                count += 1;
                break;
            }
            if (z == 0)
            {
                cout << "\n";
                break;
            }
        }
        if (!z)
            break;
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
    vector<pair<int, int>> nonRedundantEdges; // For creating an array of non-redundant edges.
    for (int i = 0; i < V + 1; i += 1)
        for (int j = 0; j < V + 1; j += 1)
            adjM[i].push_back(0);
    for (auto edge : edges)
    {
        if (adjM[edge.first][edge.second])
            continue; // The asymmetry is intentional, to find the non-redundant edges.
        adjM[edge.first][edge.second] = 1;
        nonRedundantEdges.push_back({edge.first, edge.second});
    }

    // #############################################
    // Calculating the number of triangles in the graph.
    int u, v, w; // u, v, and w form a triangle.
    count = 0;
    for (auto edge : nonRedundantEdges)
    {
        u = edge.first, v = edge.second;
        for (int w = 0; w < V + 1; w += 1)
            if (w != u and w != v and (adjM[u][w] or adjM[w][u]) and (adjM[v][w] or adjM[w][v]))
                count += 1;
    } // TC: O(m*d), m = number of edges, d = maximum of degrees of all the vertices of the graph, SC: O(V^2).
    count /= 3;

    cout << "The number of triangles in the given graph is: " << count << ".\n";

    return 0;
}