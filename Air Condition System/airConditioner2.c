/*
Code with thread and core implementation
Uses the heat equation
C = COLD
H = HOT
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <math.h>

#define N 7
pthread_mutex_t lock;
int i, j, k;
int row, column;
float tempQty[N][N];
char temp[N][N] =   {{'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                     {'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                     {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
                     {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
                     {'C', 'C', 'H', 'H', 'H', 'C', 'C'},
                     {'C', 'C', 'C', 'C', 'C', 'C', 'C'},
                     {'C', 'C', 'C', 'C', 'C', 'C', 'C'}};

//Print Matrix Function with a certain format
void printMatrix(float tempQty[N][N]){
    // int i, j;
    for (i=0;i<N;i++){
        for(j=0;j<N;j++){
            if (tempQty[i][j]<10){
                printf("| %1.1f ", tempQty[i][j]);
            }
            else{
                printf("|%1.1f ", tempQty[i][j]);
            }
        }
        printf("|\n");
    }
}

//Prints a matrix of chars
void printMatrixChar (){
    for (i=0;i<N;i++){
        for(j=0;j<N;j++){
            printf("|%c", temp[i][j]);
        }
        printf("|\n");
    }
}

//Randomly assigns values to the matrix
void *asignacionValores(void *a){
    
    pthread_mutex_lock(&lock);
    for (i=0;i<N;i++){
        for(j=0;j<N;j++){
            while(tempQty[i][j] == -1){// && tempQty[row][column] != -1
                row = rand()%7;
                column = rand()%7;
                if (temp[row][column] == 'C'){
                    tempQty[row][column] = (rand()%610)/10.0f;
                }
                else if (temp[row][column] == 'H'){
                    tempQty[row][column] = ((rand()%410)+600)/10.0f;
                }
                
            }
        }
    }
    pthread_mutex_unlock(&lock);
}

//Fills matrix with -1
void fillZeros(float tempQty[N][N]){
    for (i=0;i<N;i++){
        for(j=0;j<N;j++){
            tempQty[i][j] = -1.0;
        }
    }
}


int main(int argc, char** argv){

    srand(time(NULL));
    int thread_num = 1;
    
    if (argc > 1){
        thread_num = atoi(argv[1]);//checks the first argument
    }

    if (pthread_mutex_init(&lock, NULL) !=0){
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_t threads[thread_num];
   
    printf("The next Matrix shows if an area is cold or hot:\n");
    printMatrixChar();
    fillZeros(tempQty);

    for(int i=0; i<thread_num; i++){
        pthread_create(&threads[i], NULL, asignacionValores, NULL);//creates the threads
    }
    for(int i=0; i<thread_num; i++){
        pthread_join(threads[i], NULL);//joins the values of the threads
    }
    // asignacionValores(tempQty);
    printf("The next Matrix has the different values of temperatures:\n");
    printMatrix(tempQty);

    pthread_mutex_destroy(&lock);

    //Heat Equation
    int max_iter_time = 600; //10 hours * 60 min = 600 min
    int alpha = 2;
    int delta_x = 1;
    float delta_t = (pow(delta_x,2)) / (4 * alpha);
    float gamma = (alpha * delta_t) / (pow(delta_x,2));

    int u[max_iter_time][N][N];
    omp_set_num_threads(thread_num);//takes the amount of threads the user wants to use
    #pragma omp parallel for private(k,i,j) shared (u, tempQty)
    for (i = 0 ; i<max_iter_time-1;i++){
        for (j = 0 ; j<N;j++){
            for (k = 0 ; k<N;k++){
                u[i][j][k] = tempQty[j][k];// i =time, and j,k =temperature
            }
        }
    }

    #pragma omp parallel for private(k,i,j) shared (u)
    for (i = 0 ; i<max_iter_time-1;i++){
        for (j = 1; j<N-1;j++){
            for (k = 1 ; k<N-1;k++){
                u[i + 1][j][k] = gamma * (u[i][j+1][k] + u[i][j-1][k] + u[i][j][k+1] + u[i][j][k-1] - 4*u[i][j][k]) + u[i][j][k];
            }
        }
    }

    printf("\nDear manager, \n\n");
    printf("This is the heat equation for the factory: \n");
    // printf("y = %d * (%f / (%d)^2)\n", alpha, delta_t, delta_x);
    printf("u[i + 1][j][k] = gamma * (u[i][j+1][k] + u[i][j-1][k] + u[i][j][k+1] + u[i][j][k-1] - 4*u[i][j][k]) + u[i][j][k])\n");
    printf("The forecast of the heat at the factory for the incoming\n10 hours can be found in the document attached.\n");
    printf("We must turn on fans in <X> amount of hours to avoid\narriving to Max temperature (70 C average) in the factory.\n\nThanks!");
    return 0;
}
