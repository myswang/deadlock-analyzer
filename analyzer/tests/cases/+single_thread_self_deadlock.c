#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutexA;

void bad_func() {
    pthread_mutex_lock(&mutexA);
    pthread_mutex_lock(&mutexA);
    pthread_mutex_unlock(&mutexA);
}

int main() {
    bad_func();

    return 0;
}
