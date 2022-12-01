/**
 * Read N, K from file
 * Split N numbers to K processes
 * Log if Number is a perfect number
 * Maintain a set and then return that to parent
 * Parent finally prints all perfect numbers found
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int is_perfect(int x);
void make_k_processes(int N, int K);

int main(void)
{
    int N, K;
    FILE* f = fopen("inp.txt", "r");
    fscanf(f, "%d %d", &N, &K);
    fclose(f);

    printf("N = %d, K = %d\n", N, K);
    make_k_processes(N, K);
    return 0;
}

void make_k_processes(int N, int K){
    int i;
    for(i = 0; i < K; i++){
        if(fork() == 0){
            // Child process
            for(int x = 0; x < N/K; x++)
            {
                int num = (i * N/K) + x;
                if(is_perfect(num))
                {
                    printf("P%d : %d is a perfect number\n", i, num);
                    // Need to make this a separate function, it should have an array of size N/K
                    // Return contents of array to parent then
                }
            }
            exit(0);
        }
    }
    wait(NULL);

int is_perfect(int x)
{
    int i, sum = 0;
    for(i = 1; i < x; i++){
        if(x % i == 0){
            sum += i;
        }
    }
    return sum == x;
}