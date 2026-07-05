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
    cout << "The number of triangles in the given graph is " << count << ".\n";

    return 0;
}