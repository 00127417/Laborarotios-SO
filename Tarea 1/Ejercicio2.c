#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct timeval t0 , t1;
double average = 0.,averageProcess = 0.0;

//Thread Time 
void *time_thread(void *arg ) {
    gettimeofday(&t1 , NULL);
    unsigned int ut1 = t1. tv_sec *1000000+ t1. tv_usec ;
    unsigned int ut0 = t0. tv_sec *1000000+ t0. tv_usec ;
    average += (ut1 -ut0 );

    return NULL;
}

//Process Time 
void *process( ) {
    
    int id = -1;
    gettimeofday(&t0, NULL);
    for(int i = 0; i < 100; i++){
        id = fork();
        if(id == 0)
        return 0;
    }
    printf ("SubProcess Time Microseconds: "); 
    if(id != 0){
        gettimeofday(&t1, NULL);
        unsigned int ut1 = t1.tv_sec*1000000+t1.tv_usec;
        unsigned int ut0 = t0.tv_sec*1000000+t0.tv_usec;
        printf("%f\n", (double)(ut1-ut0)/100);
    }
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t thread;
    for ( int i = 0 ; i < 100 ; i++ ) {
        gettimeofday (&t0 , NULL);
        pthread_create (&thread, NULL , time_thread , NULL);
        pthread_join(thread, NULL);
    }
    printf ("Thread Time Microseconds: "); 
    printf (" %f\n", ( average /100.0) ); 

    process();


    return 0;
}
