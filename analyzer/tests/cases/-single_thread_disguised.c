#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutexA;
pthread_mutex_t mutexB;

void* thread_function(void* arg) {
    return NULL;
}

void sequentialFunction1() {
    pthread_mutex_lock(&mutexA);
    pthread_mutex_lock(&mutexB);
    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
}

void sequentialFunction2() {
    pthread_mutex_lock(&mutexB);
    pthread_mutex_lock(&mutexA);
    pthread_mutex_unlock(&mutexA);
    pthread_mutex_unlock(&mutexB);
}

int main() {
    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);

    pthread_t thread;
    pthread_create(&thread, NULL, thread_function, NULL);

    sequentialFunction1();
    sequentialFunction2();

    pthread_join(thread, NULL);

    pthread_mutex_destroy(&mutexA);
    pthread_mutex_destroy(&mutexB);

    return 0;
}
