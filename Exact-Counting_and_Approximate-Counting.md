# Exact counting and Approximate counting

## Exact counting

- Count the exact number of triangles in the graph.
- This count is used for calculating the clustering coefficient, fraud detection, social network analysis, and querying graph databases for their exact properties.

## Clustering coefficient

For a node ‘u’ of a graph, if ‘m’ is the actual number of triangles containing ‘u’ and ‘n’ is the possible number of triangles that can contain ‘u’, then the clustering coefficient of u = m/n.

### Example

Consider node ‘u’ has 4 neighbors ‘A’, ‘B’, ‘C’, and ‘D’ and the actual edges between them are A-B, B-C, and C-D. Then clustering coefficient of u = 3 / 4C2 = 3/6 = 0.5.

Global clustering coefficient: average over all nodes in the graph.

### Real world application (Example: Community detection)

- High clustering coefficient → tight knit community (a friends group where everyone knows everyone).
- Low clustering coefficient means the node is part of less number of triangles. Example: Spam/Fraud accounts are a part of very few triangles (no real social ties). If a node 'u' has very few neighbors, then an approximate triangle counting algorithm may report that it is part of no traingles, when in actual, it could be part of one. And getting to know about that one triangle is crucial so fraud activities can be detected and stopped, which approximate counting may not help us in doing.
- Because criminals scatter their transactions across Bank X, Bank Y, and Bank Z, no single bank can see the full triangle on its own. To fight this, countries have created centralized systems (FIU) that merge the data from all banks into one master graph. Using exact triangle counting, we can construct a subgraph from this master graph based on the clustering co-efficient (requires exact triangle counting at least in this case) (approximate triangle counting can't help as it only deals macroscopically with the graph, not at a node level) of each node and also based on the banks' sense of which accounts could be fake (based on transactions, assets, etc.). Now this sub-graph could be analyzed for fraud detection.
- Approximate algorithms are designed for macro-level analysis (getting a quick, high-level bird's-eye view of the whole graph). They are completely blind at the micro-level (inspecting individual nodes).

- Protien-protien interaction networks. It's the human body! We can't compromise on exactness.
- K-truss and link recommendation engines.

## Approximate counting

Estimate the number of triangles — close enough, but not exact. Trades accuracy for speed.

### How it works — Sampling:

- Pick a random sample of edges or nodes
- Count triangles in the sample
- Scale up to estimate total count

Example: If you sample 1% of edges and find 100 triangles, estimate total = 10,000.

Use case: WWW graph — too large for exact counting. Understanding the web graph's clustering helps improve search algorithms — pages in dense triangle clusters are likely more authoritative on a topic.
