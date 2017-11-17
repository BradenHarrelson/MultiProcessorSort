/*
Braden Harrelson
Midwestern State University
High Performance Computing
SERIAL IMPLEMETATION
*/
/*
					-INSTRUCTIONS-
	A. COMPILE:mpicc -o ParallelProphetsSerialexe SerialProject.c
	B. EXECUTE: qsub mpiJobSer
*/
const long SIZE = 1024;  //  size of the set of nums were going to use 
#include <stdio.h>			 														   //|
#include <stdlib.h>							   		                      			 
#include "timer.h" //to use timer													   //|
void quicksort(int [],long,long); //quicksort prototype                                
													                                   //| CHANGE BOTH NUMBERS
int main()																			   //|
{																					   //|
	double start,finish; //For time                                                  
	FILE *infile;	//File pointer	    
	int * set;																   //|
	set = malloc(SIZE*sizeof(int));	 //Allocate memory to hold an array of numbers of SIZE		                        
	long i;		                                  	                                 
	infile = fopen("1024nums.txt", "r");   //Open the file that contains the numbers                                  
	for(i=0; i < SIZE; i++) //read in file and put into array to be sorted
	{
		fscanf(infile, "%d , ", &set[i]);   // read in the numbers and insert in to the set array

	}
	
	GET_TIME(start); //start the timer
	quicksort(set,0,SIZE - 1 );      //CALLS QUICKSORT
	GET_TIME(finish); //stop timer
	//LOOP TO PRINT THE SORTED ARRAY
	for( i = 0; i < SIZE; i++)
	{
		printf("%d ", set[i]);
	}
	fclose(infile); //close file
	printf("\n QuickSort Time Elaspsed: %f Seconds", finish-start);
	free(set);
	return 0;
}
//*******************************************************************

// quicksort::quicksort()

// Parameters: int array[] , long left, long right 

// Use the quicksort algorithm that relys on a pivot point.

// No return value (but changes the array passed in)

//********************************************************************
void quicksort(int array[],long left,long right)
{
	long pivot,j,temp,i;
	if (left < right)
	{
		pivot = left; //set pivot to left
		i = left; //set i to the left 
		j = right; //set j to the right
		while (i < j)
		{
			while(array[i]<=array[pivot]&&i<right)
                 i++;
             while(array[j]>array[pivot])
                 j--;
             if(i<j){
                 temp=array[i]; //copy array[i] to temp value
                  array[i]=array[j]; 
                  array[j]=temp;
             }
		}
	temp = array[pivot];
	array[pivot] = array[j]; //move pivot
	array[j] = temp;
	quicksort(array,left,j-1); //Recursive call for [left to j-1]
	quicksort(array,j+1,right);//Recursive call for [j+1 to right]
	}
}
