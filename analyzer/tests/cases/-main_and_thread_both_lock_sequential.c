#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutexA, mutexB;

void *thread_fn(void *arg) {
    pthread_mutex_lock(&mutexB);
    pthread_mutex_lock(&mutexA);
    pthread_mutex_unlock(&mutexA);
    pthread_mutex_unlock(&mutexB);
    return NULL;
}

int main() {
    pthread_t t;

    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);

    pthread_mutex_lock(&mutexA);
    pthread_mutex_lock(&mutexB);
    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);

    pthread_create(&t, NULL, thread_fn, NULL);
    pthread_join(t, NULL);

    return 0;
}