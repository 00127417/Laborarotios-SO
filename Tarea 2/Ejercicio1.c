#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include<string.h>
sem_t sem; 

void* thread_hola(void* arg) 
{   
    char *msg = "Hola ";

    sem_wait(&sem); 
    for(int i = 0; i < strlen(msg); i++){
        printf("%c",msg[i]);
        fflush(stdout);
        usleep(100000);
    }
    sem_post(&sem); 
} 


void* thread_mundo(void* arg) 
{  
    char *msg = "mundo\n";

    sem_wait(&sem); 
    for(int i = 0; i < strlen(msg); i++){
        printf("%c",msg[i]);
        fflush(stdout);
        usleep(100000);
    }
    sem_post(&sem); 
}

int main() 
{ 
    sem_init(&sem, 0, 1); 
    pthread_t tid[2]; 
    for(int i= 0; i<2;i++){
      pthread_create(&tid[0],NULL,thread_hola,NULL);
      pthread_create(&tid[1],NULL,thread_mundo,NULL);
    }
    for(int e= 0; e<2;e++){
      pthread_join(tid[e],NULL); 
    }
    sem_destroy(&sem); 
    return 0; 
} 