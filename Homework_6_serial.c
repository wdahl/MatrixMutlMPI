#include <stdio.h>
#include <stdlib.h>
#include "timing.h"

//perfomrs matrix multiplication on the matrices passed to the method
double **matmul(double **A, double **B, int n){
  int i, j, k;//indices of the entries in the matrices

  //Allocates memory for result matrix
  double **C = (double **)malloc(n * sizeof(double *));  

  //loops through matrices and perfomrs matrix multiplication
  for(i=0; i<n; i++){
    C[i] = (double *)malloc(n * sizeof(double));
    for(j=0; j<n; j++){
      for(k=0; k<n; k++){
        C[i][j] = C[i][j] + (A[i][k]*B[k][j]);
      }
    }
  }

  return C;//returns resulting matrix
}

int main(){
  FILE *fp = fopen("inputMatrix.csv", "r");
  char matrixSize_str[10];
  int i=0, j=0, k=0;
  char c;
  char entry_str[25];
  double entry;
  fgets(matrixSize_str, 10, fp);
  int matrixSize = atoi(matrixSize_str);

  double **matrix = (double **)malloc(matrixSize * sizeof(double *));
  for(i=0; i<matrixSize; i++){
    matrix[i] = (double *)malloc(matrixSize * sizeof(double));
  }

  i=0;

  c = fgetc(fp);
  while(c != EOF){
    if(c == ','){
      sscanf(entry_str, "%lf", &entry);
      matrix[i][j] = entry;
      j++;
      entry_str[0] = '\0';
      k=0;
    }

    else if(c == '\n'){
      sscanf(entry_str, "%lf", &entry);
      matrix[i][j] = entry;
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

  timing_start();
  matmul(matrix, matrix, matrixSize);
  timing_stop();
  print_timing();

  char comma[2];
  fp = fopen("outputMatrix_serial.csv", "w");
  fprintf(fp, "%d\n", matrixSize);
  for(i=0;i<matrixSize;i++)
    {
      sprintf(comma, "%s", "");
      for(j=0;j<matrixSize;j++)
        {
          fprintf(fp, "%s%lf",comma,matrix[i][j]);
          sprintf(comma, "%s", ",");
        }
    
      fprintf(fp, "\n");
    }

  fclose(fp);
  return 0;
}
