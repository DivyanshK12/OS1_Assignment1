#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define FILENAME_SIZE 100

int is_perfect(int x); // function to check if a number is perfect
void make_k_processes(int N, int K, int **parentBuffer); // function to create K processes
void process(int N, int K, int i, int *buffer); // function to be executed by each process
int *create_shared_memory(size_t size); // function to create shared memory

int main(void)
{
    int N, K;
    FILE *f = fopen("inp.txt", "r");
    fscanf(f, "%d %d", &N, &K);
    fclose(f);

    int *parentBuffer[K];
    for (int i = 0; i < K; i++)
        parentBuffer[i] = create_shared_memory(N / K * sizeof(int)); // create shared memory for each process, store the address in parentBuffer

    make_k_processes(N, K, parentBuffer); // create K processes

    FILE *f2 = fopen("out.txt", "w"); // open output file
    for (int i = 0; i < K; i++)
    {
        fprintf(f2, "P%d: ", i);
        for (int j = 0; j < N / K; j++)
        {
            if (parentBuffer[i][j] == 1)
                fprintf(f2, "%d ", i * N / K + j); // print the perfect numbers
        }
        fprintf(f2, "\n");
    }
    fclose(f2);
    return 0;
}

void make_k_processes(int N, int K, int **parentBuffer)
{
    for (int i = 0; i < K; i++)
    {
        pid_t pid = fork(); // create a child process
        if (pid == 0) 
        {
            process(N, K, i, parentBuffer[i]); // child process executes process function
            exit(0); // child process exits
        }
    }
    for (int i = 0; i < K; i++)
    {
        pid_t p = wait(NULL); // wait for all child processes to finish 
        printf("Child process %d exited\n", p); // print the pid of the child process that exited
    }
}

void process(int N, int K, int i, int *buffer)
{
    char fileName[FILENAME_SIZE];
    sprintf(fileName, "out%d.txt", i); // create a file name
    FILE *f = fopen(fileName, "w"); // open the file

    for (int j = 0; j < N / K; j++)
    {
        int num = (i * N / K) + j; // calculate the number to be checked
        int perfection = 0; // variable to store the result of the check
        if (is_perfect(num))
        {
            fprintf(f, "%d is a Perfect Number\n", num); // print the result to the file
            perfection = 1; // set the result to 1
        }
        else
            fprintf(f, "%d is not a Perfect Number\n", num); // print the result to the file
        buffer[j] = perfection; // store the result in the shared memory
    }
    fclose(f);
}
int is_perfect(int x)
{
    int sum = 0; // variable to store the sum of the divisors
    for (int i = 1; i < x; i++)
    {
        if (x % i == 0) // check if i is a divisor of x
        {
            sum += i; // add i to the sum
        }
    }
    return sum == x; // return 1 if the sum is equal to x, 0 otherwise
}

int *create_shared_memory(size_t size)
{
    int protection = PROT_READ | PROT_WRITE;     // can be read and written
    int visibility = MAP_SHARED | MAP_ANONYMOUS; // only parent process and its children can access the shared memory
    return mmap(NULL, size, protection, visibility, -1, 0);
}