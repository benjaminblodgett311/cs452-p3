#include <stdlib.h>
#include <sys/time.h> /* for gettimeofday system call */
#include "lab.h"
#include "stdio.h"

/**
 * @brief Standard insertion sort that is faster than merge sort for small array's
 *
 * @param A The array to sort
 * @param p The starting index
 * @param r The ending index
 */
static void insertion_sort(int A[], int p, int r)
{
  int j;

  for (j = p + 1; j <= r; j++)
    {
      int key = A[j];
      int i = j - 1;
      while ((i > p - 1) && (A[i] > key))
        {
	  A[i + 1] = A[i];
	  i--;
        }
      A[i + 1] = key;
    }
}


void mergesort_s(int A[], int p, int r)
{
  if (r - p + 1 <=  INSERTION_SORT_THRESHOLD)
    {
      insertion_sort(A, p, r);
    }
  else
    {
      int q = (p + r) / 2;
      mergesort_s(A, p, q);
      mergesort_s(A, q + 1, r);
      merge_s(A, p, q, r);
    }

}

void merge_s(int A[], int p, int q, int r)
{
  int *B = (int *)malloc(sizeof(int) * (r - p + 1));

  int i = p;
  int j = q + 1;
  int k = 0;
  int l;

  /* as long as both lists have unexamined elements */
  /*  this loop keeps executing. */
  while ((i <= q) && (j <= r))
    {
      if (A[i] < A[j])
        {
	  B[k] = A[i];
	  i++;
        }
      else
        {
	  B[k] = A[j];
	  j++;
        }
      k++;
    }

  /* now only at most one list has unprocessed elements. */
  if (i <= q)
    {
      /* copy remaining elements from the first list */
      for (l = i; l <= q; l++)
        {
	  B[k] = A[l];
	  k++;
        }
    }
  else
    {
      /* copy remaining elements from the second list */
      for (l = j; l <= r; l++)
        {
	  B[k] = A[l];
	  k++;
        }
    }

  /* copy merged output from array B back to array A */
  k = 0;
  for (l = p; l <= r; l++)
    {
      A[l] = B[k];
      k++;
    }

  free(B);
}

double getMilliSeconds()
{
  struct timeval now;
  gettimeofday(&now, (struct timezone *)0);
  return (double)now.tv_sec * 1000.0 + now.tv_usec / 1000.0;
}


/**
 * Breaks array into num_thread blocks and sorts each block with a thread.
 * AI Use: Assisted By AI
 */
void mergesort_mt(int *A, int n, int num_thread)
{
  const int block_offset = n / num_thread;
  parallel_args args[num_thread];
  pthread_t threads[num_thread];

  for (int i = 0; i < num_thread; i++)
  {
    args[i].A = A;
    args[i].start = i * block_offset;
    args[i].end = (i + 1) * block_offset;
    if (i == num_thread - 1)
      args[num_thread - 1].end += n % block_offset;

    if (pthread_create(&threads[i], NULL, parallel_mergesort, &args[i]))
    {
      fprintf(stderr, "ERROR: pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < num_thread; i++)
    pthread_join(threads[i], NULL);

  if (num_thread < 2)
    return;

  int runs = num_thread;
  while (runs > 1) {
      int next = 0;
      for (int i = 0; i + 1 < runs; i += 2) {
          int p = args[i].start;
          int q = args[i].end - 1;
          int r = args[i + 1].end - 1;

          merge_s(A, p, q, r);

          args[next].start = p;
          args[next].end = r + 1;
          next++;
      }

      if (runs % 2 == 1) {
          args[next++] = args[runs - 1];
      }

      runs = next;
  }
}

/**
 * Calls mergesort_s with given void* args
 * AI Use: Written By AI
 */
void *parallel_mergesort(void *args)
{
  parallel_args *arg = (parallel_args *)args;
  mergesort_s(arg->A, arg->start, arg->end - 1);
  return NULL;
}

