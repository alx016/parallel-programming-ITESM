/*
Con este código se puede calcular el valor de pi a través del número de 'puntos' que están
dentro de la circunferencia entre el número de puntos total. 
*/
/* Referencias:
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/mutex-thread.c
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/findCircle.c
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define NUM_THREADS  16

//Variables
pthread_mutex_t lock;
int circleCount = 0;
int npoints = 10000;
double radius = 1;
int pointsThread;

//Checks if the coordinate is inside or outside the circle
int findCircle(double x, double y){
    double findCir = (radius*radius) - ((x*x) + (y*y));
    if (findCir>=0){
        return 1;
    }
    else{
        return 0;
    }
}

void *Count(void * a){
    int cont = 0;
    pthread_mutex_lock(&lock);
    for (int i=1;i<=pointsThread;i++){
        double xcoordinate = rand()/(RAND_MAX+1.0);
        double ycordinate = rand()/(RAND_MAX+1.0);
        int circ=findCircle(xcoordinate,ycordinate);
        if (circ==1){
            circleCount = circleCount + 1;
        }
        else{
            cont++;
        }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main (int argc, char *argv[]){
    clock_t start, end;
    double cpuTime;
    srand(time(NULL));
    start = clock();
    if (pthread_mutex_init(&lock, NULL) !=0){
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_t threads[NUM_THREADS];
    pointsThread= npoints/NUM_THREADS;

    for(int i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, Count, NULL);
    }

    for(int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    double pi = 4.0*(double)circleCount/(double)npoints;
    end = clock();
    cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("pi = %f\nTime used=%f",pi, cpuTime);
    pthread_mutex_destroy(&lock);
}
