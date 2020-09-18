#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

struct user{
    char *name;
    char *dui;
    int *age;
};

void *show_thread(void *arg){
    struct user *showUser;
    showUser = (struct user *) arg;

    printf("User name: %s\n", showUser->name);
    printf("User DUI: %s\n", showUser->dui);
    printf("User age: %ls\n", showUser->age);

}

int main (int argc, char *argv[]) {
    pthread_t thread;

    struct user newUser;
    char aux1[70];
    char aux2[10];
    int *aux3[3];
    newUser.age = 0;

    printf("Insert name: ");
    newUser.name = (char *)malloc(sizeof(aux1));
    scanf("%s", newUser.name);
    printf("Insert DUI: ");
    newUser.dui = (char *)malloc(sizeof(aux2));
    scanf("%s", newUser.dui);
    printf("Insert age: ");
    scanf("%ls", aux3);
    newUser.age = (int *)malloc(sizeof(aux3));
    newUser.age = aux3;

    //printf("User name: %s\n", newUser.name);
    //printf("User DUI: %s\n", newUser.dui);
    //printf("User age: %ls\n", newUser.age);

    pthread_create (&thread, NULL , show_thread , (void *) &newUser);
    pthread_join(thread, NULL);

}
