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
    // Creating an adjacency matrix.
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
    // Creating the order array with the help of degree array.
    vector<int> degree(V + 1, 0);
    for (auto edge : edges)
    {
        degree[edge.first] += 1;
        degree[edge.second] += 1;
    }
    vector<pair<int, int>> degree_node;
    for (int i = 0; i < V + 1; i += 1)
        if (degree[i])
            degree_node.push_back({degree[i], i});
    sort(degree_node.begin(), degree_node.end());
    vector<int> order;
    for (auto d_n : degree_node)
        order.push_back(d_n.second);

    // #############################################
    // Creating the forward neighbors array using the order array and adjacency matrix.
    vector<vector<int>> fwd_neighbors(V + 1);
    int n = order.size(); // The number of nodes present in the graph.
    for (int i = 0; i <= n - 2; i += 1)
        for (int j = i + 1; j < n; j += 1)
            if (adjM[order[i]][order[j]])
                fwd_neighbors[order[i]].push_back(order[j]);

    // #############################################
    // Calculating the number of triangles in the graph.
    vector<int> visited(V + 1, 0); // For marking.
    int u, v, w;
    count = 0;
    for (int i = 0; i < n; i += 1)
    {
        u = order[i];
        // marking all the forward neighbors of u.
        for (auto f_n_u : fwd_neighbors[u])
            visited[f_n_u] = 1;
        for (auto f_n_u : fwd_neighbors[u])
        {
            v = f_n_u;
            for (auto f_n_v : fwd_neighbors[v])
            {
                w = f_n_v;
                if (visited[f_n_v])
                    count += 1;
            }
        }

        // unmarking
        for (auto f_n_u : fwd_neighbors[u])
            visited[f_n_u] = 0;
    } // TC: O(m*max_degree), m = number of edges and max_degree is the maximum of degrees of the nodes of the graph.
    // SC: O(V^2), V is the maximum value node of the graph.

    cout << "The number of triangles in the given graph is: " << count << ".\n";

    return 0;
}