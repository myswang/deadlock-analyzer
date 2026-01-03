#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Register {
    double currentBalance;
} Register;

typedef struct Customer {
    double rewardPoints;
    double storeBalance;
} Customer;

typedef struct TransferArgs {
    Register *registerUsed;
    Customer *currentCustomer;
    double transferAmount;
} TransferArgs;

pthread_mutex_t resource1, resource2, resource3;

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

void *redeemPoints(void *vargp) {
    pthread_mutex_lock(&resource2);

    printf("Point redemption processing..\n");
    TransferArgs *targs = (TransferArgs *)vargp;
    targs->registerUsed->currentBalance -= targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_lock(&resource3);

    targs->currentCustomer->rewardPoints -= targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_unlock(&resource3);

    pthread_mutex_unlock(&resource2);
    pthread_exit(NULL);
}

void *convertPoints(void *vargp) {
    pthread_mutex_lock(&resource3);

    printf("Point conversion processing..\n");
    TransferArgs *targs = (TransferArgs *)vargp;
    targs->currentCustomer->storeBalance -= targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_lock(&resource1);

    targs->currentCustomer->rewardPoints += targs->transferAmount;
    // assume that now we are doing some sort of API call to a database
    sleep(1);

    pthread_mutex_unlock(&resource1);

    pthread_mutex_unlock(&resource3);
    pthread_exit(NULL);
}

int main() {
    struct Register todaysRegister = {3579.51};
    struct Customer bob = {300.0, 44.29};

    struct TransferArgs thread1Args = {&todaysRegister, &bob, 22.56};
    struct TransferArgs thread2Args = {&todaysRegister, &bob, 11.93};
    struct TransferArgs thread3Args = {&todaysRegister, &bob, 5.1};

    pthread_mutex_init(&resource1, NULL);
    pthread_mutex_init(&resource2, NULL);
    pthread_mutex_init(&resource3, NULL);

    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;

    pthread_create(&thread1, NULL, payment, &thread1Args);
    pthread_create(&thread2, NULL, redeemPoints, &thread2Args);
    pthread_create(&thread3, NULL, convertPoints, &thread3Args);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("%f\n", todaysRegister.currentBalance);
    printf("%f\n", bob.storeBalance);
    printf("%f\n", bob.rewardPoints);

    return 0;
}
