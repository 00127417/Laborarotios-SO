#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 

sem_t sem; 

struct data
{
    int id;
};



void* thread_init(void* arg) 
{   
    struct data *stack = (struct data *) arg;

    sem_wait(&sem); 
    for(int i = 0; i < 35; i++){
        stack[i].id = 0;
    }
    sem_post(&sem); 
} 


void* thread_fill(void* arg) 
{  
    struct data *stack = (struct data *) arg;
    
    sem_wait(&sem); 
    for(int i = 0; i < 35; i++){
        stack[i].id = i+1;
    }
    sem_post(&sem); 
}

void* thread_show(void* arg) 
{  
     struct data *stack = (struct data *) arg;
    
    sem_wait(&sem); 
    for(int i = 0; i < 35; i++){
        
        printf("%d ",stack[i].id);
    }
    printf("\n");
    sem_post(&sem); 
}


int main() 
{ 
    struct data stack[35];
    
    sem_init(&sem, 0, 3); 
    pthread_t tid[3]; 
    pthread_create(&tid[0],NULL,thread_init,(void *) &stack);
    pthread_create(&tid[1],NULL,thread_fill,(void *) &stack);
    pthread_create(&tid[2],NULL,thread_show,(void *) &stack);


    for(int e= 0; e<3;e++){
        pthread_join(tid[e],NULL); 
    }
    sem_destroy(&sem); 
    return 0; 
} 