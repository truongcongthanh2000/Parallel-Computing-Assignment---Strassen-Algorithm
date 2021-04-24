# Parallel Computing Assignment --- Strassen's Algorithm

This is a repo for a uni project.  
The objective is to benchmark Strassen's algorithm for matrix multiplication, in its original serial version and our own parallel versions of them implemented with the support of C++ OpenMP, against the respective naive version.  

Contributors:
1. Nguyễn Hữu Thiện Phú  
2. Trương Công Thành  

To reproduce the results of our benchmarking results, first clone the project.  

Then run:  
	 **make**  
to compile the project.  

Next, to generate the input and model output matrices, run:  
	 **./run.exe -g**  

Finally, run:  
	 **./run.exe**  

For further tinkering, all the options for executing run.exe is listed below:
- -g: generate input and model output matrices.
- -gr: -g and then start the benchmarking.
- -i: files containing input and model output matrices will be specified later through console.
- -n \<num\>: number of times to benchmark, default is 1.

Note: the options are checked as prefix against command line arguments in the listed sequence. Any invalid option is not checked and may result in undefined behavior.