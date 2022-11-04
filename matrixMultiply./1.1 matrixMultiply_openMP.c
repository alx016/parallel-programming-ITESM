#include <stdio.h>
#define m 2
#define n 3
#define p 3
#define q 2

int main(void){
    int A[m][n] = {{1,2,3},
                   {4,5,6}}; //matrix[row][column]
    int B[p][q] = {{ 7, 8},
                   { 9,10},
                   {11,12}};
    int C[m][q];

    //Matrix multiplication
    if (n==p){
        for (int i=0; i<m;i++){
            for (int j=0;j<q;j++){
                C[i][j] = 0;
                for (int k=0;k<p;k++){
                    C[i][j] = C[i][j] + (A[i][k]*B[k][j]);
                }
            }
        }
    }
    else {
        printf("Error, non matching matrix size!");
    }
    
    for (int r=0; r<m;r++){
        for (int s=0;s<q;s++){
            printf("%d  ",C[r][s]);
        }
        printf("\n");
    }
    
    return 0;
}
