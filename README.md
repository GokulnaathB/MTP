# MTP Topic: Parallelization of triangle counting graph algorithms of undirected graphs.

## What is triangle counting?
A triangle counting graph algorithm counts the number of cycles made of exactly three edges in a given undirected graph. 

## Applications in various domains

### Social Networks
- Friend recommendation: if person A and person B have many common friends, suggest they connect.
- Community detection: dense triangle regions indicate groups.

### Spam and Fraud Detection
- Spam accounts have very few triangles (no real social ties).
- Fradulent transactions form unusual triangle patterns in financial networks.

### Biology
- Protein-protein interaction networks: triangles indicate **functional modules**.
- Brain connectivity networks: triangle density reveals **neural clusters**.

### Graph Metrics
- Computing **clustering coefficient** — ratio of actual triangles to possible triangles for a node. Core metric in network science.

### Cybersecurity
- **Botnet** detection.
- Intrusion detection in network traffic graphs.

## Approaches to the counting
1. The naive one
   - In the naive approach, we consider every possible set of 3 vertices of the graph and check whether they are all  connected to each other. If so, then we count it as a triangle.
   - What makes this approach inefficient is that we also consider every possible permutation (3! of them) for every possible set of 3 vertices of the graph, causing a multitude of redundant checks in case of real-world graphs and hence, the inefficiency. And as a result, we've to divide the final count value by 6.
   - The TC of this approach is O(n^3) and the SC is O(V^2). n = the number of vertices in the grpah, V = maximum of the values of all the vertices.

2. The Edge Iterator approach
   - In this approach, we consider every edge of the graph and for each one of them, we count the number of common neighbor vertices of the vertices encompassing the edge. And it's obvious that the number of common neighbor vertices is equal to the number of triangles formed with that particular as one of its sides.
   - The redundancy present in this approach is that the number of common neighbors is counted for all the three sides of a triangle, again causing massive computation in case of real-world graphs and hence, inefficient, but not as inefficient as the naive approach. And as a result, we've to divide the final count value by 3.
   - The TC of this approach is O(m*d) and the SC is O(V^2). m = number of edges in the graph, d = maximum of degrees of all the vertices of the graph, and V = maximum of the values of all the vertices.

3. The DAG Node Iterator approach
