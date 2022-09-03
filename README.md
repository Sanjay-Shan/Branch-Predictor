## Branch Predictor in C++

This project was accomplished as a part of the course work in Advanced Computer Architecture. The aim of the project was to soley design a 3 set of branch predictors and interpret it performance with different criterias.
1. Smith Branch predictor
2. Bimodal branch Predictor
3. Gshare branch predictorake 

Once the predictors were designed ,the end goal was to make them clear the validation runs in the form of a trace file, which were provided as a part of the project.
Namely 3 sets of trace files were provided in total:
1. gcc_trace
2. jpeg_trace
3. perl_trace

All the respective trace files have been attached with this repository for your reference. 

Finally a custom code was written in python to demonstrate the performance of each of the branch predictors w.r.t the misprediction rate that varies with the change in the number of bits in the counter.
The same python code for generating respective graphs has been attached here with. And each of the graphs pertaining to each of the predictors have been wrapped inside a unique folder.
