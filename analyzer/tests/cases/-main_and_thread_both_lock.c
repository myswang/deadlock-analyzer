#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutexA;
pthread_mutex_t mutexB;

void *worker(void *arg) {
    pthread_mutex_lock(&mutexB);
    sleep(1);
    pthread_mutex_unlock(&mutexB);
    return NULL;
}

int main() {
    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);

    pthread_t t1;
    pthread_create(&t1, NULL, worker, NULL);

    pthread_mutex_lock(&mutexA);
    sleep(1);
    pthread_mutex_unlock(&mutexA);

    pthread_join(t1, NULL);
    return 0;
}