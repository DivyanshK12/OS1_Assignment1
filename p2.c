#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int is_perfect(int x);
void make_k_processes(int N, int K, int** parentBuffer);
void process(int N, int K, int i, int* buffer);
int *create_shared_memory(size_t size);

int main(void)
{
    int N, K;
    FILE *f = fopen("inp.txt", "r");
    fscanf(f, "%d %d", &N, &K);
    fclose(f);

    int* parentBuffer[K];
    for(int i = 0; i < K; i++)
        parentBuffer[i] = create_shared_memory(N/K * sizeof(int));
    
    make_k_processes(N, K, parentBuffer);

    for(int i = 0; i <K; i++)
    {
        for(int j = 0; j < N/K; j++)
            printf("%d ", parentBuffer[i][j]);
        printf("\n");
    }
    return 0;
}

void make_k_processes(int N, int K, int** parentBuffer)
{
    for (int i = 0; i < K; i++)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            // char* filename = (char*)malloc(20 * sizeof(char));
            // strcat(filename, "out");
            // strcat(filename, '0' + i);
            // strcat(filename, ".txt");
            // printf("%s\n", filename);
            process(N, K, i, parentBuffer[i]);
            exit(0);
        }
    }
    for(int i = 0; i < K; i++)
    {
        pid_t p = wait(NULL);
        printf("Child process %d exited\n", p);
    }
}

void process(int N, int K, int i, int* buffer)
{
    for (int x = 0; x < N / K; x++)
    {
        int num = (i * N / K) + x;
        int perfection = 0;
        if (is_perfect(num))
        {
            perfection = 1;
        }
        buffer[x] = perfection;
    }
}
int is_perfect(int x)
{
    int i, sum = 0;
    for (i = 1; i < x; i++)
    {
        if (x % i == 0)
        {
            sum += i;
        }
    }
    return sum == x;
}

int* create_shared_memory(size_t size)
{
    int protection = PROT_READ | PROT_WRITE;     // can be read and written
    int visibility = MAP_SHARED | MAP_ANONYMOUS; // only parent process and its children can access the shared memory
    return mmap(NULL, size, protection, visibility, -1, 0);
}