# FFT
Some thoughts on how to parallelize the general Cooley–Tukey FFT algorithm, mainly to benchmark the performance of different libraries like OpenMP, Intel TBB, OpenCL and C++11 threads.
From Wikipedia:
"The Cooley–Tukey algorithm, named after J.W. Cooley and John Tukey, is the most common fast Fourier transform (FFT) algorithm. It re-expresses the discrete Fourier transform (DFT) of an arbitrary composite size N = N1N2 in terms of N1 smaller DFTs of sizes N2, recursively, to reduce the computation time to O(N log N) for highly composite N (smooth numbers)."
At the last level (before merging the sub DFTs) it  becomes effectively a DFT on a single point, independent of all others. This seems to suggest that the problem is highly parellelizable. 
