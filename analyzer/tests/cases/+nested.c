#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutexA, mutexB;

void lockA_then_B() {
    pthread_mutex_lock(&mutexA);
    sleep(1);
    pthread_mutex_lock(&mutexB);
    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
}

void lockB_then_A() {
    pthread_mutex_lock(&mutexB);
    sleep(1);
    pthread_mutex_lock(&mutexA);
    pthread_mutex_unlock(&mutexA);
    pthread_mutex_unlock(&mutexB);
}

void *thread1(void *arg) {
    lockA_then_B();
    return NULL;
}

void *thread2(void *arg) {
    lockB_then_A();
    return NULL;
}

int main() {
    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}