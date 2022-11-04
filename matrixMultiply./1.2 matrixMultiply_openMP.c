#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define N 1000

int A[N][N];
int B[N][N];
int C[N][N];


int main(int argc, char** argv){
    int i, j, k;
    int thread_num= 1;

    if (argc > 1){
        thread_num = atoi(argv[1]);
    }

    omp_set_num_threads(thread_num);//takes the amount of threads the user wants to use

    //Creating two vectors full of 2's and 3's
    for (i=0; i<N;i++){
        for (j=0;j<N;j++){
            A[i][j] = 2;
            B[i][j] = 3;
        }
    }

    //Matrix multiplication using OpenMP
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for (i=0; i<N;i++){
        for (j=0;j<N;j++){
            for (k=0;k<N;k++){
                C[i][j] = C[i][j] + (A[i][k]*B[k][j]);
            }
        }
    }

    //Prints Matrix
    for (i=0; i<N;i++){
        for (j=0;j<N;j++){
            printf("%d  ",C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
