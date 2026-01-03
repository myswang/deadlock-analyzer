#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutexA;

void *bad_func(void *arg) {
    pthread_mutex_lock(&mutexA);
    pthread_mutex_lock(&mutexA);
    pthread_mutex_unlock(&mutexA);

    return NULL;
}

int main() {
    pthread_t t1;

    pthread_create(&t1, NULL, bad_func, NULL);

    pthread_join(t1, NULL);

    return 0;
}
