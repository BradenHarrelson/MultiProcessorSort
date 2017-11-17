/*
Braden Harrelson
Midwestern State University
High Performance Computing
Parallel Implementation
*/
/*
					-INSTRUCTIONS-
	A. COMPILE:mpicc -o ParallelProphetsParallelexe ParallelProphetsParallel.c
	B. EXECUTE: qsub mpiJobpPara1N4P         ----- 1 Node 4 Processors
*/
const long SIZE = 1024; //  size of the set of nums were going to use

#include <stdio.h> 
#include <stdlib.h> 
#include <stdbool.h> 
#include <mpi.h>             //Allows use of the MPI library
void quicksort(int[], long, long); //quicksort prototype     
int * mergeSubset(int set1[], int set2[], int size, int my_rank); //mergeSubset prototype
int main() {
    int comm_sz, my_rank, k, j, dum;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, & my_rank);   //Rank of the process
    MPI_Comm_size(MPI_COMM_WORLD, & comm_sz);    //How many processors are being used?
    double start, finish; //For time                                                   
    FILE * infile; //File pointer	  
    int * set = NULL; //big set to be allocated
    int * localSet = NULL; //local set to be allocated to each process
    int * newLocal = NULL; //array to be allocated to save return of mergeSubset function
    int * setRecv = NULL; //array to be recved from a receive
    int partition = SIZE / comm_sz; //split the data up evenly
    long i;

    if (my_rank == 0) {
      set = malloc(SIZE * sizeof(int)); //malloc big data set
      infile = fopen("1024nums.txt", "r");
      int temp;
      for (i = 0; i < SIZE; i++) //read in file and put into array to be sorted
      {
        dum = fscanf(infile, "%d , ", & temp);
        set[i] = temp;
      }
      fclose(infile); //close file
    }
    if (my_rank == 0) { // P0 starts the timer
      start = MPI_Wtime();
    }
    localSet = malloc(partition * sizeof(int));
	/* Scatter set to localset with size partition */
    MPI_Scatter(set, partition, MPI_INT, localSet, partition, MPI_INT, 0, MPI_COMM_WORLD);
    quicksort(localSet, 0, partition - 1); //quicksort each subset

    int step = 1;
    while (step < comm_sz) {
      if (my_rank % (2 * step) == 0) { //matching partners up for communication, by even and odd
        if (my_rank + step < comm_sz) {
          MPI_Recv( & partition, 1, MPI_INT, (my_rank + step), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          setRecv = malloc(partition * sizeof(int));
          MPI_Recv(setRecv, partition, MPI_INT, my_rank + step, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          newLocal = mergeSubset(localSet, setRecv, partition, my_rank); //recive the newLocal from mergeSubset function
          localSet = malloc(2 * partition * sizeof(int));
          partition = partition * 2; //increment partition
          if (partition != SIZE) {
            for (i = 0; i < partition; i++) {
              localSet[i] = newLocal[i];
            }
          } else {

            if (my_rank == 0) {

              //LOOP TO PRINT THE SORTED ARRAY
              finish = MPI_Wtime(); //get finish time
              for (i = 0;
                (i < SIZE); i++) {
                printf("%d ", newLocal[i]);
              }
              printf("\nQuickSort Time Elaspsed: %f Seconds\n", finish - start);
            }
          }

        }
      } else {
        int sendTo = my_rank - step; //which arrau to send to
        if(sendTo % (2 * step) == 0){
	        MPI_Send( & partition, 1, MPI_INT, sendTo, 0, MPI_COMM_WORLD);
        	MPI_Send(localSet, partition, MPI_INT, sendTo, 1, MPI_COMM_WORLD);
        }
      }
      step = step * 2;

    }
    //finish
    free((void * ) localSet);
    MPI_Finalize();
    return 0;
  }
//********************************************************************

// quicksort::quicksort()

// Parameters: int array[] , long left, long right 

// Use the quicksort algorithm that relys on a pivot point.

// No return value (but changes the array passed in)

//********************************************************************
void quicksort(int array[], long left, long right) {
  long pivot, j, temp, i;
  if (left < right) {
    pivot = left; //set pivot to left
    i = left; //set i to the left 
    j = right; //set j to the right
    while (i < j) {
      while (array[i] <= array[pivot] && i < right)
        i++;
      while (array[j] > array[pivot])
        j--;
      if (i < j) {
        temp = array[i]; //copy array[i] to temp value
        array[i] = array[j];
        array[j] = temp;
      }
    }
    temp = array[pivot];
    array[pivot] = array[j]; //move pivot
    array[j] = temp;
    quicksort(array, left, j - 1); //Recursive call for [left to j-1]
    quicksort(array, j + 1, right); //Recursive call for [j+1 to right]
  }
}
//********************************************************************

// mergeSubset::mergeSubset()

// Parameters: int set1[] , int set2[] , int size, int my_rank 

// Combining the two sub sorted arrays

// returns a larger sorted array

//********************************************************************
int * mergeSubset(int set1[], int set2[], int size, int my_rank) {
  int i = 0, j = 0, k = 0, l = 0, q = 0; //Variables to use for loops (some might not me used / debugging)
  int * subset = NULL;
  subset = malloc((size * 2) * sizeof(int)); //allocte size*2 memory for subset
  while (i < size && j < size) {
  	//set1's element is bigger
    if (set1[i] < set2[j]) {
      subset[k] = set1[i];
      k++;
      i++;
    } else //set2[j] < set1[i]
    {
      //set2's element is bigger
      subset[k] = set2[j];
      k++;
      j++;
    }
    if (i == size) { 
      while (j < size) {
        subset[k] = set2[j];
        k++;
        j++;
      }
    }
    if (j == size) {
      while (i < size) {
        subset[k] = set1[i];
        k++;
        i++;
      }
    }

  } //end of while
  return subset;
}
