#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutexA, mutexB, mutexC;

void thread_inner() {
    pthread_mutex_lock(&mutexB);
    pthread_mutex_lock(&mutexC);
    pthread_mutex_unlock(&mutexC);
    pthread_mutex_unlock(&mutexB);
}

void* thread_fn(void* arg) {
    thread_inner();
    return NULL;
}

void main_inner() {
    pthread_mutex_lock(&mutexA);
    pthread_mutex_lock(&mutexB);
    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
}

int main() {
    pthread_t t;

    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);
    pthread_mutex_init(&mutexC, NULL);

    pthread_create(&t, NULL, thread_fn, NULL);

    main_inner();

    pthread_join(t, NULL);
    return 0;
}