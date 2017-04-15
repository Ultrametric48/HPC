
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mpi.h> // include mpi library header
#include "secret_function.h"
#include <omp.h>


void sample_rand(const double a, const double b ,const int dim, double *x) {

  for(int i=0;i<dim;++i) {
    double tmp = ((double) rand())/((double) RAND_MAX);
    tmp = (b-a)*tmp + a;
    x[i] = tmp;
  }
}



double logL(double *x, int dim) { 

  double f = 0.0;

  for(int i=0;i<dim-1;++i) {
    f += (1. - x[i])*(1. - x[i]) + 100.*(x[i+1] - x[i]*x[i])*(x[i+1] - x[i]*x[i]);
  }

  f = -f;

  return f;
}

int main(int argc, char **argv)
{

  double totalStart;
  double totalEnd;
  double ringStart;
  double ringEnd;
  double MPIReductionStart;
  double MPIReductionEnd;
  double SampleStart;
  double SampleEnd;
  double evaluationStart;
  double evaluationEnd;

  totalStart = omp_get_wtime();
	
  int rank, size;

  MPI_Init (&argc, &argv);      // initializes MPI
  MPI_Comm_rank (MPI_COMM_WORLD, &rank); // get current MPI-process ID. O, 1, ...
  MPI_Comm_size (MPI_COMM_WORLD, &size); // get the total number of processes

  const unsigned int N = (10*10*10*10*10*10*10)/(size); //1.e7; // for testing
  const int dim = 10;
  double x[dim];  // array of random numbers
  double max = -1;
  double min = 1000;
  double SecretFunctionValue;

  srand(time(NULL) * rank); // each MPI process gets a unique seed


  SampleStart = omp_get_wtime();
  for(unsigned int i=1; i<=N; ++i) {
    sample_rand(-512,512,dim,x);
   // SecretFunctionValue = secret_function(sample_rand);
    evaluationStart = omp_get_wtime();
    double f_i = secret_function(x[0],x[1]); //exp(logL(x,dim));
    evaluationEnd = omp_get_wtime();
    //printf("Secret Function Evaluation Time is %lf \n", evaluationEnd-evaluationStart);
    //printf("Process %d, sample %d, Secret Function Evaluation %lf \n",rank, i, f_i);
    if( f_i < min) {
      min = f_i;
    }
  }
  SampleEnd = omp_get_wtime();

 //printf("Sample Time is %lf \n", SampleEnd-SampleStart);

 //printf("For Process %d Local min is %lf \n", rank, min);




//use min in shared memory
//Thanks to Tazkera for some very good suggestions on how to structure the ring topology.

  int send_junk = min;
  int rec_junk;
  MPI_Status status;
  double globalMin = 0;



  ringStart = omp_get_wtime();
// This next if-statment implemeents the ring topology
  // the last process ID is size-1, so the ring topology is: 0->1, 1->2, ... size-1->0
  // rank 0 starts the chain of events by passing to rank 1
  if(rank==0) {
    // only the process with rank ID = 0 will be in this block of code.
    MPI_Send(&send_junk, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); //  send data to process 1
   //MPI_Recv(&rec_junk, 1, MPI_INT, size-1, 0, MPI_COMM_WORLD, &status); // receive data from process size-1
  }
  else if( rank == size-1) {
    MPI_Recv(&rec_junk, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status); // recieve data from process rank-1 (it "left" neighbor")
        if(rec_junk <= min){
        min = rec_junk;
        }
	send_junk = min;

        MPI_Send(&send_junk, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // send data to its "right neighbor", rank 0
}
  else {

    MPI_Recv(&rec_junk, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status); // recieve data from process rank-1 (it "left" neighbor")
  if(rec_junk <= min){
        min = rec_junk;
        }
        send_junk = min;
  MPI_Send(&send_junk, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD); // send data to its "right neighbor" (rank+1)
  }

if(rank == 0){

MPI_Recv(&rec_junk, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
if(rec_junk <= min){
        min = rec_junk;
        }

//printf("Ring topology output global minima %lf for rank %d size %d \n", min, rank , size);

}

 // printf("Process %d sent %i and recieved %i\n", rank, send_junk, rec_junk );

  //MPIReductionStart = omp_get_wtime();

  //MPI_Reduce(&min, &globalMin, 1 , MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

 // MPIReductionEnd = omp_get_wtime();

  if(rank == 0){

  printf("Process ID (Rank) is %d, for size %d. Global min is %lf \n", rank, size, globalMin);

   }

  ringEnd = omp_get_wtime();

 //printf("globalmin is %lf,\n", min);
  

  MPI_Finalize(); // programs should always perform a "graceful" shutdown


  totalEnd = omp_get_wtime();

 // printf("The total wall time is %lf \n", totalEnd-totalStart);
  //printf("The ring topology time is %lf \n ", ringEnd-ringStart);
 // printf("The MPI reducttion time is %lf \n", MPIReductionEnd-MPIReductionStart);

   printf("gloabl min is %lf \n", min);

  return 0;

}


