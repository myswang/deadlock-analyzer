#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t m1, m2;

void safe_function() {
    pthread_mutex_lock(&m1);
    pthread_mutex_unlock(&m1);

    pthread_mutex_lock(&m2);
    pthread_mutex_unlock(&m2);
}

int main() {
    safe_function();

    return 0;
}
