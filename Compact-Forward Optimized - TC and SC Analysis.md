# Time and Space Complexity Analysis of the Various Compact-Forward Optimized Algorithms

| Method              | TC         | SC                             | Parallelizable |
| ------------------- | ---------- | ------------------------------ | -------------- |
| Visited array + CSR | O(m.Δ)     | O(V + m)                       | NO             |
| Linear scan + CSR   | O(m.(Δ^2)) | O(m)                           | YES            |
| fwd_bits + CSR (1)  | O(m.V/64)  | O(V^2 / 8)                     | YES            |
| fwd_bits + CSR (2)  | O(m.V/64)  | O(V^2 / 8) Better in practice. | YES            |

'm' is the number of edges (for each pair of vertices that are connected, only one direction considered [vertex with lower id to vertex with higher id]).

'V' is the total number of nodes in the given graph.

'Δ' is the maximum of forward degrees of all the vertices of the graph.

## Linear scan + CSR

One edge in CSR is stored as one integer in the fwd_neighbors array, and an integer is 4 bytes. So, one edge = 4 bytes.

Let’s say you have a RAM of 8 GB. Then,

```text
m x 4 bytes = 8 GB = 8 x 10^9 bytes
=> m = 2 x 10^9 => m = 2 billion edges.
```

So with 8 GB RAM, the linear scan + CSR approach can handle graphs with up to roughly 2 billion edges.

1. For a dense graph where almost all possible edges exist:

   ```text
   V x (V-1)/2 = 2 billion => V is approximately 63,000.
   ```

2. For a sparse graph where each node has only 100 neighbors on an average:

   ```text
   V x 100/2 = 2 billion => V = 40 million.
   ```

=> The number of edges alone does not tell you the number of nodes — it depends heavily on the graph's density.

## fwd_bits + CSR

Let’s say you have a RAM of 8 GB. Then,

```text
V x V/8 bytes = 8 GB = 8 x 10^9 bytes
=> V^2 = 64 x 10^9 => V is approximately 250,000.
```

So with 8 GB RAM, the fwd_bits + CSR approach can handle graphs with only up to 250,000 nodes.

The fwd_bits + CSR approach is better for dense graphs with about 250,000 nodes and the Linear scan + CSR approach is better for sparse graphs with about 40 million nodes. All this is applicable on a system having a RAM of 8 GB.

Time complexity wise also, the Linear scan + CSR approach is better for sparse graphs and the fwd_bits + CSR approach is better for dense graphs.

But since most of the real-world graphs are sparse graphs, the Linear scan + CSR approach is the way to go. The fwd_bits + CSR approach can be used for very specific real-world applications where dense graphs rule, for example, protein-protein interaction networks, brain connectivity networks, co-authorship graphs in narrow academic fields, high frequency trading networks, etc. But all of these are small and dense, not large and dense! But even for large and dense graphs (1 million nodes => 10^12 edges), the storage requirements are in petabytes, which is nearly impossible to have.
