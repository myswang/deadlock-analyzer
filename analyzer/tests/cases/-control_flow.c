#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutexA, mutexB;

void maybe_lock(int condition) {
    if (condition) {
        pthread_mutex_lock(&mutexA);
    } else {
        pthread_mutex_lock(&mutexB);
    }

    if (condition) {
        pthread_mutex_unlock(&mutexA);
    } else {
        pthread_mutex_unlock(&mutexB);
    }
}

void loop_lock() {
    for (int i = 0; i < 3; ++i) {
        pthread_mutex_lock(&mutexA);
        pthread_mutex_unlock(&mutexA);
    }
}

void *thread1(void *arg) {
    maybe_lock(1);
    return NULL;
}

void *thread2(void *arg) {
    maybe_lock(0);
    loop_lock();
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}