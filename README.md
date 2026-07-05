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
   - This approach is a slight improvement on the Edge Iterator approach. In the Edge Iterator, for each edge (u,v) we scan all vertices to find common neighbors. And we also end up checking the same triangles multiple times from different edges. There's a lot of redundancy involved here and the following will make it make sense.
   - In DAG Node Iterator approach, we first order the vertices in ascending order based on their degree. Then we make a DAG as follows: for each undirected edge (u, v), direct it from the lower ranked vertex to the higher ranked vertex.
   - Now we count the traingles as follows:
     ```
     count = 0
     for each node u (in order): // outer loop
     mark all forward neighbors of u // loop 1
     for each forward neighbor v of u: // loop 2
        for each forward neighbor w of v: // inner loop
           if w is marked:
                  count += 1
     unmark all // loop 3
     ```
     A "forward neighbor" of u means a neighbor that has higher degree than u.
     This approach eliminates the redundancy by imposing an ordering on the vertices and only looking "forward" in that ordering. As a result, each triangle is counted exactly once.
     - Intuition: Consider a vertex 'u' of the graph. Mark all the forward neighbors of this vertex. Now pick a forward neighbor 'v' of 'u' (Note that (u, v) form an edge). Now pick a forward neighbor 'w' of 'v' and check whether it is marked. If it is marked, then we have a triangle as 'w' is a forward neighbor of both 'u' and 'v'.

     ## Time Complexity Analysis
     Let 'm' be the number of edges in the graph.
     - Since we've constructed a graph in which we've directed edges from lower degree vertex to higher degree vertex, loop 1 and loop 3, across all the iterations of the outer loop, run 'm' number of times. *O(m)*
     - Loop 2 runs 'm' number of times as the sum of the number of forward neighbors of all the vertices of the graph equals the number of edges. The inner loop runs |forward_neighbors[v]| number of times. forward_neighbors[v] contains vertices with degree higher than 'v' that 'v' is connected to. That's a subset of all neighbors of 'v'. So, |forward_neighbors[v]| <= degree(v) <= d, d = maximum of degrees of all the vertices of the graph. *O(m*d)*
     - To get the order, we're doing ascending order sort of the vertices based on their degree and that takes *O(n*log(n))* time, n = total number of vertices in the graph.
     - ### TC = O(m + m*d + n*log(n)). But m*d dominates over n*logn and m for any reasonably dense graph. Therefore, the TC = O(m*d).
