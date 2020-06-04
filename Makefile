run: Homework_6_serial.out Homework_6_distributed.out generateSquareMatrix.out

Homework_6_distributed.out: Homework_6_distributed.c timing.c
        mpicc Homework_6_distributed.c timing.c -o Homework_6_distributed.out

Homework_6_serial.out: Homework_6_serial.c timing.c
        gcc -g Homework_6_serial.c timing.c -o Homework_6_serial.out

generateSquareMatrix.out: generateSquareMatrix.c
        gcc generateSquareMatrix.c -o generateSquareMatrix.out

clean:
        rm *.out
