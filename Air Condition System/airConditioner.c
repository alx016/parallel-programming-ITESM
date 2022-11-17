/*

C = COLD
H = HOT
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

#define N 7
pthread_mutex_t lock;
int i, j;
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

    // omp_set_num_threads(thread_num);//takes the amount of threads the user wants to use

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
    return 0;
}



// char temp[N][N] =   {{'H', 'H', 'C', 'C', 'C', 'C', 'C'},
//                      {'H', 'H', 'C', 'C', 'C', 'C', 'C'},
//                      {'H', 'H', 'C', 'C', 'C', 'C', 'C'},
//                      {'H', 'H', 'C', 'C', 'C', 'C', 'C'},
//                      {'H', 'H', 'C', 'C', 'C', 'C', 'C'},
//                      {'H', 'H', 'H', 'C', 'C', 'C', 'C'},
//                      {'H', 'H', 'H', 'C', 'C', 'C', 'C'}};

// for (i=0;i<N;i++){
//     for(j=0;j<N;j++){
//         if (temp[i][j] == 'C'){
//             tempQty[i][j] = (rand()%610)/10.0f;
//         }
//         else if (temp[i][j] == 'H'){
//             tempQty[i][j] = ((rand()%410)+600)/10.0f;
//         }
//     }
// } 
