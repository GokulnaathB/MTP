# Testing our compact forward optimized algorithms using ECL graphs.

- ECL graphs (`.egr` files) are graphs stored in binary CSR format — developed by Martin Burtscher at Texas State University for GPU graph algorithm research.
- The key file you need is `ECLgraph.h` — a header file that handles reading .egr files. Download it from [ECLgraph.h](https://userweb.cs.txstate.edu/~burtscher/research/ECLgraph/ECLgraph.h)
- [ECLgraphs](https://userweb.cs.txstate.edu/~burtscher/research/ECLgraph/) provides links to download some sample graphs in ECL format. They are all undirected (i.e., symmetric) and do not contain loops (self-edges). Some have edge weights, none of which are negative or zero.

# How to use them?

```cpp
#include<bits/stdc++.h>
#include "ECLgraph.h"

int main(int argc, char *argv[]) {
    ECLgraph g = readECLgraph(argv[1]);

    // g.nodes — number of nodes
    // g.edges — number of edges
    // g.nindex — row pointer array (CSR)
    // g.nlist — flat adjacency list array (CSR)

    // Your triangle counting code here

    freeECLgraph(g);
    return 0;
}
```

Run it as:
`./your_program.cpp graph.egr`

- amazon0601.egr has got 403394 nodes. It has got 3986507 triangles. Use this to test your Linear scan + CSR algorithm, which can handle a sparse graph of about 40 million nodes.
- internet.egr has got 124651 nodes. It has got 19523 triangles. Use this to test your fwd_bits + CSR algorithm, which can handle graphs with only upto 250000 nodes.
