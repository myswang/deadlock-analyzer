#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lockA;
pthread_mutex_t lockB;

void helper1() {
    pthread_mutex_lock(&lockA);
    pthread_mutex_lock(&lockB);
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
}

void helper2() {
    pthread_mutex_lock(&lockB);
    pthread_mutex_lock(&lockA);
    pthread_mutex_unlock(&lockA);
    pthread_mutex_unlock(&lockB);
}

int main() {
    helper1();
    helper2();

    return 0;
}
