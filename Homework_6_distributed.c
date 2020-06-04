#include <stdio.h>
#include <stdlib.h>
#include "timing.h"
#include <mpi.h>

//reads matrix from file
void read_matrix(double *matrix, int matrixSize, FILE *fp)
{
  int i=0, j=0, k=0;
  char c;
  char entry_str[25];
  double entry;

  c = fgetc(fp);
  while(c != EOF){
    if(c == ','){
      sscanf(entry_str, "%lf", &entry);
      matrix[i*matrixSize + j] = entry;
      j++;
      entry_str[0] = '\0';
      k=0;
    }
    else if(c == '\n'){
      sscanf(entry_str, "%lf", &entry);
      matrix[i*matrixSize + j] = entry;
      i++;
      j=0;
      entry_str[0] = '\0';
      k=0;
    }
    else{
      entry_str[k] = c;
      k++;
    }

    c = fgetc(fp);
  }

  fclose(fp);
}

//writes matrix to file
void write_matrix(double *resultMatrix, int matrixSize)
{
  int i, j;
  char comma[2];
  FILE *fp = fopen("outputMatrix_distributed.csv", "w");
  fprintf(fp, "%d\n", matrixSize);
  for(i=0;i<matrixSize;i++)
  {
    sprintf(comma, "%s", "");
    for(j=0;j<matrixSize;j++)
    {
      fprintf(fp, "%s%lf",comma,resultMatrix[i*matrixSize + j]);
      sprintf(comma, "%s", ",");
    }

    fprintf(fp, "\n");
  }

  fclose(fp);
}

int main()
{
  //gets matrix size from file
  FILE *fp = fopen("inputMatrix.csv", "r");
  char matrixSize_str[10];
  fgets(matrixSize_str, 10, fp);
  int matrixSize = atoi(matrixSize_str);

  double chunk[matrixSize*matrixSize]; //proccesses chunk of matrix to calulate
  double matrix[matrixSize*matrixSize];//matrix
  double resultMatrix[matrixSize*matrixSize];//result matrix for each proccess

  int size, rank; //num of procs and current proc
  int num_of_rows, i, j, k; //number of rows to give the process to do

  MPI_Status status;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  //chekcs if the matrix size is divisable by the number of processes given
  if(matrixSize%size != 0)
  {
    printf("Number of processes numst be divisable by the matrix size");
    MPI_Abort(MPI_COMM_WORLD, 0);
    exit(1);
  }

  num_of_rows = matrixSize/size;//total rwos for process to calculate

  //master proc reads matrix
  if(rank == 0)
  {
    read_matrix(matrix, matrixSize, fp);
  }
  //scatter matrix across procs for the rows to be calcuated
  MPI_Scatter(&matrix, num_of_rows*matrixSize, MPI_DOUBLE, &chunk, num_of_rows*matrixSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  //broad cast matrix to procs
  MPI_Bcast(&matrix, matrixSize*matrixSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  timing_start();
  //multiply matrices
  for(i=0; i<num_of_rows; i++){
    for(j=0; j<matrixSize; j++){
      resultMatrix[i*matrixSize + j] = 0.0;
      for(k=0; k<matrixSize; k++){
        resultMatrix[i*matrixSize + j] += chunk[i*matrixSize + k] * matrix[k*matrixSize + j];
      }
    }
  }

  //gather results from procs intot he matrix in master
  MPI_Gather(&resultMatrix, num_of_rows*matrixSize, MPI_DOUBLE, &matrix, num_of_rows*matrixSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  timing_stop();

  //master writes the matrix to a file
  if(rank == 0)
  {
    write_matrix(matrix, matrixSize);
    print_timing();
  }

  MPI_Finalize();
  return 0;
}
