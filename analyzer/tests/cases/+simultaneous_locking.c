#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t A, B;

void* thread1_func(void* arg) {
    pthread_mutex_lock(&A);
    pthread_mutex_unlock(&A);

    pthread_mutex_lock(&B);
    pthread_mutex_unlock(&B);
    return NULL;
}

void* thread2_func(void* arg) {
    pthread_mutex_lock(&B);
    pthread_mutex_unlock(&B);

    pthread_mutex_lock(&A);
    pthread_mutex_unlock(&A);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1_func, NULL);
    pthread_create(&t2, NULL, thread2_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
