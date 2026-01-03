#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m1;
pthread_mutex_t m2;
pthread_mutex_t m3;

void lock_m3() {
    pthread_mutex_lock(&m3);
}

void lock_m1_then_m3() {
    pthread_mutex_lock(&m1);
    lock_m3();
    pthread_mutex_unlock(&m3);
    pthread_mutex_unlock(&m1);
}

void lock_m2_then_m1() {
    pthread_mutex_lock(&m2);
    pthread_mutex_lock(&m1);
    pthread_mutex_unlock(&m1);
    pthread_mutex_unlock(&m2);
}

void *thread1(void *arg) {
    lock_m1_then_m3();
    return NULL;
}

void *thread2(void *arg) {
    lock_m2_then_m1();
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}