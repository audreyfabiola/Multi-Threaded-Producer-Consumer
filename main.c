/*
    Clarissa Audrey Fabiola Kusnadi
    2602118490
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define LOWER_NUM 1
#define UPPER_NUM 10000
#define BUFFER_SIZE 100
#define MAX_COUNT 10000

// File
FILE * all, * even, * odd;

// Number Counted
int evenCount, oddCount, allCount;

// Global Flag
int flag = 0;

/*
    | --------------------------------------|
    |               Stack                   |
    |---------------------------------------|
*/

pthread_mutex_t mutex;
pthread_cond_t stack_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t stack_not_empty = PTHREAD_COND_INITIALIZER;

int stack[BUFFER_SIZE];
int stack_top = 0;

void push(int x){
    pthread_mutex_lock(&mutex);
    while(stack_top == BUFFER_SIZE){ // If it's full => WAIT
        pthread_cond_wait(&stack_not_full, &mutex); // Wait for signal
    }
    stack[stack_top++] = x;
    pthread_cond_signal(&stack_not_empty); // Indicates it's not empty
    pthread_mutex_unlock(&mutex);
}

int pop(int id){
    int ret = -1;

    pthread_mutex_lock(&mutex);

    repeat:
    while (stack_top == 0) {
        if(flag){ // Event Finished
            pthread_mutex_unlock(&mutex);
            return -1;
        }
        pthread_cond_wait(&stack_not_empty, &mutex);
    }
    if(stack[stack_top - 1] % 2 != id){ // Check even or odd
        pthread_cond_wait(&stack_not_empty, &mutex);
        goto repeat;
    }

    ret = stack[--stack_top];

    pthread_cond_signal(&stack_not_full);
    pthread_cond_signal(&stack_not_empty);
    pthread_mutex_unlock(&mutex);

    return ret;
}
/* ================== END OF STACK ===================== */


/*
    | --------------------------------------|
    |         Customer & Producer           |
    |---------------------------------------|
*/

// Number Generator Function
void * producerFunction(){
    allCount = 0;
    for (int i = 0; i < MAX_COUNT; i++){
        int num = (rand() % (UPPER_NUM - LOWER_NUM + 1)) + LOWER_NUM;
        fprintf(all, "%d\n", num);
        push(num);
        allCount++;
    }
    while(stack_top > 0){} // Loop until stack becomes empty
    flag = 1;
    pthread_cond_signal(&stack_not_empty);
}

// Even Function
void * customer1Function(){
    while(evenCount + oddCount != MAX_COUNT){
        printf("%d %d %d\n", evenCount, oddCount, stack_top);
        int num = pop(0); // pop Even
        if(num == -1) break; // Invalid
        if(num % 2 == 1){
            printf("Error Number not Matched");
            exit(1);
        }
        fprintf(even, "%d\n", num);
        evenCount++;
    }
}

// Odd Function
void * customer2Function(){
    while(evenCount + oddCount != MAX_COUNT){
        printf("%d %d %d\n", evenCount, oddCount, stack_top);
        int num = pop(1); // pop Odd
        if(num % 2 == 0){
            printf("Error Number not Matched");
            exit(1);
        }
        if(num == -1) break; // Invalid
        fprintf(odd, "%d\n", num);
        oddCount++;
    }
}

/* ================== END OF PRODUCER  & CUSTOMER ================== */

int main() {
    // Initialize
    all = fopen("all.txt", "w");
    even = fopen("even.txt", "w");
    odd = fopen("odd.txt", "w");
    srand(time(NULL));
    flag = 0;

    if (all == NULL || even == NULL || odd == NULL)
        printf("Can't open or create text files\n");

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init error");
        exit(EXIT_FAILURE);
    }

    clock_t start_time, end_time;
    double elapsed_time;

    start_time = clock(); // Record start time

    // Create the Threads
    pthread_t producerThread, customerThread1, customerThread2;
    if (pthread_create(&producerThread, NULL, producerFunction, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&customerThread1, NULL, customer1Function, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&customerThread2, NULL, customer2Function, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Wait Threads to finish
    if (pthread_join(producerThread, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    printf("Procedure Done\n");

    if (pthread_join(customerThread1, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    printf("Customer 1 Done\n");

    if (pthread_join(customerThread2, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    printf("Customer 2 Done\n");

    end_time = clock(); // Record end time

    elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    // Destroy
    pthread_mutex_destroy(&mutex);

    printf("Elapsed time: %f seconds\n", elapsed_time);
    printf("All Count: %d\n", allCount);
    printf("Even Count: %d\n", evenCount);
    printf("Odd Count  : %d\n", oddCount);

    printf("All threads have finished successfully.\n");
    fclose(all);
    fclose(even);
    fclose(odd);

    return 0;
}

