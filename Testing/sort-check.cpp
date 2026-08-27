#include <bits/stdc++.h>
#include "ECLgraph.h"
using namespace std;

int main(int argc, char *argv[])
{ // fwd_bits + CSR

    // #############################################
    // Creating an ECLgraph object of the input graph.
    ECLgraph g = readECLgraph(argv[1]);
    int V = g.nodes;

    // #############################################
    // Sort check.
    int l, r;
    for (int i = 0; i < g.nodes; i += 1)
    {
        l = g.nindex[i], r = g.nindex[i + 1] - 1;
        for (int j = l; j < r; j += 1)
            if (g.nlist[j] > g.nlist[j + 1])
            {
                cout << "Not sorted!";
                break;
            }
    }

    cout << "Sorted!";

    return 0;
}