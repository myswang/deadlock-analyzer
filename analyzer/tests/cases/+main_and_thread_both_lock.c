#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutexA, mutexB, mutexC;

void* thread1(void* arg) {
    pthread_mutex_lock(&mutexA);
    sleep(1);
    pthread_mutex_lock(&mutexB);
    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
    return NULL;
}

void* thread2(void* arg) {
    pthread_mutex_lock(&mutexB);
    sleep(1);
    pthread_mutex_lock(&mutexC);
    pthread_mutex_unlock(&mutexC);
    pthread_mutex_unlock(&mutexB);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);
    pthread_mutex_init(&mutexC, NULL);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_mutex_lock(&mutexC);
    sleep(1);
    pthread_mutex_lock(&mutexA);
    pthread_mutex_unlock(&mutexA);
    pthread_mutex_unlock(&mutexC);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}