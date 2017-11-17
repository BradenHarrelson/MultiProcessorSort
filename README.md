# Sorting
## Overall Description
Contains Serial and Parallel Version (openMPI) of the Quick-Sort algorithm. Used to sort a large amount of randomized numbers (up to 2^28). A performance analysis was performed to show display execution time, speed-up, and efficiency.

## Serial Code
Sequentially sorts a large array of randomized numbers from a file using Quick-Sort. Execution time was recorded using the time.c and timer.h files.
    
    *LOOK AT SortSerial.c
## Parallel Code
Parallel implementation written in C using openMPI. Execution time was recorded using MPI's time function MPI_Wtime(). 
    
    *LOOK AT SortParallel.c
### Master Process
The master process reads in the large array of randomized numbers from the exact same file used in the serial implementation. The master process then distributes a portion of the array to each individual process. The master process is in charge of recording the execution time.
### Slave Processes
The slaves processes recieves a portion of unsorted randomized numbers then performs quick-sort on the allocated portion of numbers. Each slave process is "partnered up" with another slave process. These partners send thier locally sorted array to a function that merges the two locally sorted array into one bigger array. This is repeated till the last two processes merge into the last sorted array. By working from the bottom on the tree you can visualize the merging function using 8 processes.

![](https://i.imgur.com/N45h2Fi.png)

## Performance Analysis Breakdown
### Execution Time
Notice it takes 49 minutes to sequentially sort 2^28 random numbers with more processors acting in parallel sorting takes a fraction of the time. For example, 16 processors took 30 seconds to sort the same set of numbers.
![](https://i.imgur.com/f7aO0yd.png)
### Speed-Up
Speedup increases with the more processors being used. When the dataset becomes greater so does the need for more processors.
![](https://i.imgur.com/24YbZvE.png)
### Efficiency
Efficiency is greatest at 16 processors. This is due to the architecture of the cluster that was used in the data collection. Each node in the cluster had 16 cores. When the number of processors is greater than 16 the efficiency decreased beacuse the communication overhead between two seperate nodes. 
![](https://i.imgur.com/nB9yI3v.png)
