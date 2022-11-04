//This code solves a matrix multiplication using openMP, the twist in here is that the user can define the amount of threads to use
//and therefore, reduce the amount of time used to solve it.  The user can also change the matrix size.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

//default matrix size
#define N 1000 

int A[N][N];
int B[N][N];
int C[N][N];


int main(int argc, char** argv){
    int i, j, k, n;
    int thread_num= 1;

    if (argc > 1){
        thread_num = atoi(argv[1]);
        n = atoi(argv[2]);//checks for a second input, which is the size of the matrix
    }
    else{
        n=N;
    }

    A[n][n];
    B[n][n];
    C[n][n];

    omp_set_num_threads(thread_num);//takes the amount of threads the user wants to use

    //Creating two vectors full of 2's and 3's
    for (i=0; i<n;i++){
        for (j=0;j<n;j++){
            A[i][j] = 2;
            B[i][j] = 3;
        }
    }

    //Matrix multiplication using OpenMP
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (i=0; i<n;i++){
        for (j=0;j<n;j++){
            for (k=0;k<n;k++){
                C[i][j] = C[i][j] + (A[i][k]*B[k][j]);
            }
        }
    }

    for (i=0; i<n;i++){
        for (j=0;j<n;j++){
            printf("%d  ",C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
