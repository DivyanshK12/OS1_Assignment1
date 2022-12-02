#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct threadData
{
    int N;
    int K;
    int i;
    int **threadResults;
} threadData;

void *checkPerfect(void *data);
int isPerfectNumber(int x);

int main(void)
{
    FILE *inputFile = fopen("inp.txt", "r");
    int N, K;
    fscanf(inputFile, "%d %d", &N, &K);
    fclose(inputFile);

    pthread_t threads[K];
    int *perfectNumberArray[K];

    for (int i = 0; i < K; i++)
    {
        threadData t;
        t.i = i;
        t.K = K;
        t.N = N;
        t.threadResults = &(perfectNumberArray[i]);
        pthread_create(&threads[i], NULL, checkPerfect, (void *)&t);
        pthread_join(threads[i], NULL);
    }

    FILE *outFile = fopen("out.txt", "w");
    for (int i = 0; i < K; i++)
    {
        for (int j = 0; j < N / K; j++)
        {
            int num = i * (N / K) + j;
            if (perfectNumberArray[i][j] == 1)
                fprintf(outFile, "%d : Is a perfect Number\n", num);
            else
                fprintf(outFile, "%d : Not a perfect Number\n", num);
        }
    }
    fclose(outFile);
}

void *checkPerfect(void *data)
{
    threadData *t = (threadData *)data;
    int N = t->N;
    int K = t->K;
    int i = t->i;
    *t->threadResults = (int *)calloc(N / K, sizeof(int));

    for (int x = 0; x < N / K; x++)
    {
        int num = (i * N / K) + x;
        if (isPerfectNumber(num))
            (*t->threadResults)[x] = 1;
        else
            (*t->threadResults)[x] = 0;
    }
}

int isPerfectNumber(int x)
{
    int sum = 0;
    for (int i = 1; i < x; i++)
    {
        if (x % i == 0)
            sum += i;
    }
    return sum == x;
}