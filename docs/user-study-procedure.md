# Project 2 User Study Procedure

Our user study will consists of three main components:

1. Starting questions to familiarize user
2. Working through deadlock examples
3. Going through visualizations to see if they would have helped

Although our program's primary language target may be extended to C++, we will start with C. Therefore, it would be thematic for our user study to contain snippets of C code.

## Starting Questions

First, the background of the user will be understood through a series of questions:

-   Do you understand what a mutex is?
-   Do you understand what a deadlock is?
-   Have you programmed in C or read C code before?
-   Do you have any familiarity with the `pthread.c` library?
-   Have you used any software to understand deadlock in code better?

## Examples

The user will be shown 3 code snippets, written in C with the `pthread.h` library for threads. General usage of `pthread.h` was learned through [a stack overflow post](https://stackoverflow.com/questions/27480125/simple-deadlock-example-using-pthread) which shows a simple deadlock example.

### Example 1: No Deadlock

In the procedure, the user will first be briefed on the situation:

-   Video store owner Kerry has designed their own multi-threaded transaction system for their shop.
-   We will look at instances of Kerry's program to see if there may be anything wrong with it.
-   If the user believes that they have identified an issue with the program, ask how the user would go about solving this problem.
-   If the user is unable to identify the issue with the program, tell the user the issue and ask how they would go about solving this problem.

A real-world example was chosen to ground the user in the environment and would be more valuable as our tool is being developed for practical developers.

In this first example, a deadlock is not found in the system, because although there are two threads, they occur one after another. This would show the user that there is nothing necessarily wrong with using multiple threads themselves.

```C
// Kerry's Awesome Video Store In-House Multi-threaded Transaction System in C
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
```

### Example 2: Simple Deadlock

In this next example, the code experiences a deadlock as there are two threads which are both holding a resource while waiting for another (each other).

The user will be asked the same questions as in the first example.

```C
// Keri's Awesome Video Store In-House Multi-threaded Transaction System in C
// ... (assume same structs and functions)

int main() {
    struct Register todaysRegister = {3579.51};
    struct Customer bob = {300, 44.29};

    struct TransferArgs thread1Args = {&todaysRegister, &bob, 22.56};
    struct TransferArgs thread2Args = {&todaysRegister, &bob, 11.93};

    pthread_mutex_init(&resource1, NULL);
    pthread_mutex_init(&resource2, NULL);

    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, payment, &thread1Args);
    pthread_create(&thread2, NULL, refund, &thread2Args);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("%f\n", todaysRegister.currentBalance);
    printf("%f\n", bob.storeBalance);

    return 0;
}
```

### Example 3: A More Complex Deadlock

In this example, we add a third thread and a third resource, and ask the same questions to the user. There is also a deadlock in this example.

```C
// Kerry has decided to flesh out the points system!
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
```

## Visualization Questions

After going through the examples, the user will be shown diagrams as seen here: https://www.geeksforgeeks.org/resource-allocation-graph-rag-in-operating-system/, and be asked:

-   If the user is able to match such diagrams to the example code and conceptualize them
-   If these diagrams would have helped in their approaches in understanding the errors associated with the examples
-   If the user would use these diagrams as an aid to debug deadlocks in their code
-   If the user would be interested in using a program that generates such diagrams
