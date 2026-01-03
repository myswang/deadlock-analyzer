#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Register {
    double currentBalance;
} Register;

typedef struct Customer {
    int rewardPoints;
    double storeBalance;
} Customer;

typedef struct TransferArgs {
    Register *registerUsed;
    Customer *currentCustomer;
    double transferAmount;
} TransferArgs;

pthread_mutex_t resource1, resource2;

void *payment(void *vargp) {
    pthread_mutex_lock(&resource1);

    printf("Payment processing..\n");
    TransferArgs *targs = (TransferArgs *)vargp;
    targs->currentCustomer->storeBalance -= targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_lock(&resource2);

    targs->registerUsed->currentBalance += targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_unlock(&resource2);

    pthread_mutex_unlock(&resource1);
    pthread_exit(NULL);
}

void *refund(void *vargp) {
    pthread_mutex_lock(&resource2);

    printf("Refund processing..\n");
    TransferArgs *targs = (TransferArgs *)vargp;
    targs->currentCustomer->storeBalance += targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_lock(&resource1);

    targs->registerUsed->currentBalance -= targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_unlock(&resource1);

    pthread_mutex_unlock(&resource2);
    pthread_exit(NULL);
}

int main() {
    struct Register todaysRegister = {3579.51};
    struct Customer bob = {300, 44.29};

    struct TransferArgs thread1Args = {&todaysRegister, &bob, 22.56};
    struct TransferArgs thread2Args = {&todaysRegister, &bob, 11.93};

    pthread_mutex_init(&resource1, NULL);
    pthread_t thread1;
    pthread_create(&thread1, NULL, payment, &thread1Args);
    pthread_join(thread1, NULL);

    pthread_mutex_init(&resource2, NULL);
    pthread_t thread2;
    pthread_create(&thread2, NULL, refund, &thread2Args);
    pthread_join(thread2, NULL);

    printf("%f\n", todaysRegister.currentBalance);
    printf("%f\n", bob.storeBalance);

    return 0;
}
