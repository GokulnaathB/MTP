## How to run the C++ codes?
To run the naive-parallel.cpp code, first run ```g++ -fopenmp naive-parallel.cpp -o naive-parallel``` and then run ```OMP_SCHEDULE="static" ./naive-parallel``` (OMP_SCHEDULE can take three values: static, dynamic, and guided). Make sure you have the GCC C++ compiler installed on your system. 
