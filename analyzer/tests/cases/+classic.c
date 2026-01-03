#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lockA;
pthread_mutex_t lockB;

void* thread_func1(void*arg) {
    pthread_mutex_lock(&lockA);
    pthread_mutex_lock(&lockB);
    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    return NULL;
}

void* thread_func2(void*arg) {
    pthread_mutex_lock(&lockB);
    pthread_mutex_lock(&lockA);
    pthread_mutex_unlock(&lockA);
    pthread_mutex_unlock(&lockB);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_func1, NULL);
    pthread_create(&t2, NULL, thread_func2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}
