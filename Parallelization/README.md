## How to run the C++ codes?
To run the naive-parallel.cpp code, first run ```g++ -fopenmp naive-parallel.cpp -o naive-parallel``` and then run ```OMP_SCHEDULE="static" ./naive-parallel``` (OMP_SCHEDULE can take three values: static, dynamic, and guided). Make sure you have the GCC C++ compiler installed on your system. 

## Example
```g++ -fopenmp naive-parallel.cpp -o naive-parallel``` and then 
1. ```OMP_SCHEDULE="static" ./naive-parallel```  
   OUTPUT:  
   Parallel time: 0.239 seconds.  
   Parallel: The number of triangles in the given graph is 739428.


   Sequential time: 1.046 seconds.  
   Sequential: The number of triangles in the given graph is 739428.
2. ```OMP_SCHEDULE="dynamic" ./naive-parallel```  
   OUTPUT:  
   Parallel time: 0.186 seconds.  
   Parallel: The number of triangles in the given graph is 739428.


   Sequential time: 1.068 seconds.  
   Sequential: The number of triangles in the given graph is 739428.
3. ```OMP_SCHEDULE="guided" ./naive-parallel```  
   OUTPUT:  
   Parallel time: 0.196 seconds.  
   Parallel: The number of triangles in the given graph is 739428.


   Sequential time: 1.068 seconds.  
   Sequential: The number of triangles in the given graph is 739428.

## Speedup
Speedup = Sequential time/Parallel time  
Speedup tells how many times faster is our parallel version compared to our sequential version.  

From our results with dynamic scheduling:  
Speedup = 1.068 / 0.186 is approximately 5.7.  
I've got 10 cores and 12 logical processors on my laptop. So, the ideal speed up should be 12x. Why did I get only 5.7x?! That's because of non-sequential memory accesses by threads, Thread overhead, and "Hyper-threading isn't true parallelsim."
