# Exact counting and Approximate counting

## Exact counting

- Count the exact number of triangles in the graph.
- This count is used for calculating the clustering coefficient, fraud detection, social network analysis, and querying graph databases for their exact properties.

## Clustering coefficient

For a node ‘u’ of a graph, if ‘m’ is the actual number of triangles containing ‘u’ and ‘n’ is the possible number of triangles that can contain ‘u’, then the clustering coefficient of u = m/n.

### Example

Consider node ‘u’ has 4 neighbors ‘A’, ‘B’, ‘C’, and ‘D’ and the actual edges between them are A-B, B-C, and C-D. Then clustering coefficient of u = 3 / 4C2 = 3/6 = 0.5.

Global clustering coefficient: average over all nodes in the graph.

### Real world interpretation (Example: Community detection)

- High clustering coefficient → tight knit community (a friends group where everyone knows everyone).
- Low clustering coefficient loose connections. Example: →
- Spam accounts have very few triangles (no real social ties).
- Very few triangles means low clustering coefficient.

## Approximate counting

Estimate the number of triangles — close enough, but not exact. Trades accuracy for speed.

### How it works — Sampling:

- Pick a random sample of edges or nodes
- Count triangles in the sample
- Scale up to estimate total count

Example: If you sample 1% of edges and find 100 triangles, estimate total = 10,000.

Use case: WWW graph — too large for exact counting. Understanding the web graph's clustering helps improve search algorithms — pages in dense triangle clusters are likely more authoritative on a topic.
