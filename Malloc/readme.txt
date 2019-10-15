The run time for workload A is 0.000007 seconds
The run time for workload B is 0.000027 seconds
The run time for workload C is 0.000302 seconds
The run time for workload D is 0.000350 seconds
The run time for workload E is 0.000004 seconds
The run time for workload F is 0.000200 seconds

An interesting finding was the result of workload A compared to workload E. In A we were just malloc()ing 1 byte and then immediately free()ing it but in E we 
malloc() max memory capacity and also free() it immediately. It turns out that workload E was faster than A. The runtime of workload E does not make sense but 
one possible answer would be the CPU memorizing the subsequent calls to malloc() and free() and then executing these instructions quicker in E. The result 
that was not surprising was workload D having the longest runtime which could have been caused by the rand() function.