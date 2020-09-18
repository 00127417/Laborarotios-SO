#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct employee{
    char *name;
    char *lastname;
    int len;
};

void *nameThread(void *arg){
    struct employee *emply;
    emply = (struct employee *) arg;
    char  word[50];
    printf("Insert name: ");
    emply->name = (char *)malloc(sizeof(word));
    scanf("%s", emply->name);
    printf("Insert lastname: ");
    emply->lastname = (char *)malloc(sizeof(word));
    scanf("%s", emply->lastname);
    
    return NULL;
}
void * count_thread(void *arg){
    struct employee *emply;
    emply = (struct employee *) arg;


    int accum = 0;
    int i;
    for(i=0; i<strlen(emply->name); i++){
        accum += 1;
    }
    for(i=0; i<strlen(emply->lastname); i++){
        accum += 1;
    }
    emply->len = accum;
    return NULL;
}

void * answer_thread(void *arg){
    struct employee *emply;
    emply = (struct employee *) arg;

    if(emply->len > 25){
        printf("WILL TRUNCATE\n");
    }else{
        printf("NO PROBLEM\n");
    }
    
    return NULL;
}


int main(int argc, char *argv []){
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;

    struct employee emp;

    emp.len = 0;
    pthread_create(&t1, NULL, nameThread, (void *)&emp);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, count_thread, (void *)&emp);
    pthread_join(t2, NULL);
    pthread_create(&t3, NULL, answer_thread, (void *)&emp);
    pthread_join(t3, NULL);
    
}