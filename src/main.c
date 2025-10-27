#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lab.h"

/**
 * Fills an array with random integers.
 *
 * AI Use: Written By AI
 */
static void fill_rand(int *a, int n)
{
    for (int i = 0; i < n; i++) a[i] = rand();
}

/**
 * Takes an array size and thread count and sorts a random array of
 * that size with the specified amount of threads.
 *
 * AI Use: Written By AI
 */
int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: %s <array_size> <num_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int t = atoi(argv[2]);
    if (n < 0) n = 0;
    if (t < 1) t = 1;
    if (t > MAX_THREADS) t = MAX_THREADS;

    int *A = n ? (int*)malloc((size_t)n * sizeof(int)) : NULL;
    if (n && !A) return 2;

    srand((unsigned)time(NULL));
    if (n) fill_rand(A, n);

    double start = getMilliSeconds();
    mergesort_mt(A, n, t);
    double end = getMilliSeconds();

    printf("%d %f\n", t, end - start);

    free(A);
    return 0;
}

