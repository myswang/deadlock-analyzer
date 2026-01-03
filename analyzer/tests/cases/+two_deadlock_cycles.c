#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t A, B, C, D;

void *thread1(void *arg) {
    pthread_mutex_lock(&A);
    sleep(1);
    pthread_mutex_lock(&B);
    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);
    return NULL;
}

void *thread2(void *arg) {
    pthread_mutex_lock(&B);
    sleep(1);
    pthread_mutex_lock(&A);
    pthread_mutex_unlock(&A);
    pthread_mutex_unlock(&B);
    return NULL;
}

void *thread3(void *arg) {
    pthread_mutex_lock(&C);
    sleep(1);
    pthread_mutex_lock(&D);
    pthread_mutex_unlock(&D);
    pthread_mutex_unlock(&C);
    return NULL;
}

void *thread4(void *arg) {
    pthread_mutex_lock(&D);
    sleep(1);
    pthread_mutex_lock(&C);
    pthread_mutex_unlock(&C);
    pthread_mutex_unlock(&D);
    return NULL;
}

int main() {
    pthread_mutex_init(&A, NULL);
    pthread_mutex_init(&B, NULL);
    pthread_mutex_init(&C, NULL);
    pthread_mutex_init(&D, NULL);

    pthread_t t1, t2, t3, t4;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_create(&t3, NULL, thread3, NULL);
    pthread_create(&t4, NULL, thread4, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    return 0;
}