/*Con este código se puede calcular el valor de pi a través del número de 'puntos' que están
dentro de la circunferencia entre el número de puntos total. 

Referencias:
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/mutex-thread.c
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/main/labs/04/findCircle.c
https://stackoverflow.com/questions/31089952/using-rand-rand-max-1
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
    srand(time(NULL));//useful to have different random numbers
    start = clock();//initialize clock
    if (pthread_mutex_init(&lock, NULL) !=0){
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_t threads[NUM_THREADS];
    pointsThread= npoints/NUM_THREADS;

    for(int i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, Count, NULL);//creates the threads
    }

    for(int i=0; i<NUM_THREADS; i++){
        pthread_join(threads[i], NULL);//joins the values of the threads
    }

    double pi = 4.0*(double)circleCount/(double)npoints;//calculates pi
    end = clock();
    cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC; //calculates the time
    printf("pi = %f\nTime used=%f\n",pi, cpuTime);//prints pi and the time it took 
    pthread_mutex_destroy(&lock);
}
