# MatrixMutlMPI
Distributed implementation of matrix multiplication using MPI

## Compile 
To complie the code run the make file in the terminal:
```bash
make
```

## generateSquareMatrix.c
Generates a square matrix and writes to a csv. Takes 1 command line argument specifying the size of the square matrix
```bash
./generateSquareMatirx [size of matrix]
```
Output will be a csv file called "inputMatrix.csv"

## Homework_6_distributed.c
Performs matrix multiplication distributed over multipul nodes on the matrix stored in "inputMatrix.csv" using MPI
```bash
./Homework_2_parallel
```

## Homework_2_serial.c
Performs matrix multiplication on the matrix stored in "inputMatrix.csv"
```bash
./Homework_2_serial.c
```
