#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "harness/unity.h"
#include "../src/lab.h"

static int is_sorted(const int *a, int n)
{
    for (int i = 1; i < n; i++) if (a[i-1] > a[i]) return 0;
    return 1;
}

static void fill_seq(int *a, int n) { for (int i = 0; i < n; i++) a[i] = i; }
static void fill_rev(int *a, int n) { for (int i = 0; i < n; i++) a[i] = n - 1 - i; }
static void fill_const(int *a, int n, int v) { for (int i = 0; i < n; i++) a[i] = v; }
static void fill_small_domain(int *a, int n, int k) { for (int i = 0; i < n; i++) a[i] = i % k; }
static void fill_extremes(int *a, int n)
{
    for (int i = 0; i < n; i++) a[i] =
        (i % 4 == 0) ? INT_MIN : (i % 4 == 1) ? INT_MAX : (i % 4 == 2) ? -i : i;
}

void setUp(void) {}
void tearDown(void) {}

static void check_sorted_eqref(int *a, int n)
{
    int *ref = n ? malloc((size_t)n * sizeof(int)) : NULL;
    if (n) memcpy(ref, a, (size_t)n * sizeof(int));
    mergesort_s(ref, 0, n ? n - 1 : 0);
    TEST_ASSERT_TRUE(is_sorted(a, n));
    TEST_ASSERT_EQUAL_INT_ARRAY(ref, a, n);
    free(ref);
}

static void run_mt_case(int n, int threads, void (*fill)(int*,int))
{
    int *a = n ? malloc((size_t)n * sizeof(int)) : NULL;
    if (n) fill(a, n);
    mergesort_mt(a, n, threads);
    check_sorted_eqref(a, n);
    free(a);
}

void test_s_empty(void) { int *a = NULL; mergesort_s(a, 0, -1); TEST_ASSERT_TRUE(1); }
void test_s_single(void) { int a[1] = {42}; mergesort_s(a, 0, 0); TEST_ASSERT_TRUE(is_sorted(a,1)); }
void test_s_small_threshold_edges(void) { int a[3] = {2,1,2}; mergesort_s(a,0,2); TEST_ASSERT_TRUE(is_sorted(a,3)); }

void test_merge_basic(void)
{
    int a[10];
    for (int i=0;i<5;i++) a[i]=i*2;
    for (int i=0;i<5;i++) a[5+i]=i*2+1;
    merge_s(a,0,4,9);
    TEST_ASSERT_TRUE(is_sorted(a,10));
}

void test_merge_uneven(void)
{
    int a[11];
    for (int i=0;i<7;i++) a[i]=i*3;
    for (int i=0;i<4;i++) a[7+i]=i*3+1;
    merge_s(a,0,6,10);
    TEST_ASSERT_TRUE(is_sorted(a,11));
}

void test_mt_n0_t_any(void) { run_mt_case(0, 8, fill_seq); }
void test_mt_n1(void) { run_mt_case(1, 8, fill_seq); }
void test_mt_t1(void) { run_mt_case(1000, 1, fill_rev); }
void test_mt_t_eq_n_cap(void) { run_mt_case(32, 64, fill_rev); }
void test_mt_t_gt_n(void) { run_mt_case(16, 128, fill_extremes); }
void test_mt_rev_large(void) { run_mt_case(5000, 8, fill_rev); }
void test_mt_const(void) { run_mt_case(4096, 6, (void(*)(int*,int)) (void*) (fill_const)), (void)0; }

void test_mt_small_domain(void) { run_mt_case(7000, 7, (void(*)(int*,int)) (void*) (fill_small_domain)); }
void test_mt_extremes(void) { run_mt_case(6000, 5, fill_extremes); }

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_s_empty);
    RUN_TEST(test_s_single);
    RUN_TEST(test_s_small_threshold_edges);
    RUN_TEST(test_merge_basic);
    RUN_TEST(test_merge_uneven);
    RUN_TEST(test_mt_n0_t_any);
    RUN_TEST(test_mt_n1);
    RUN_TEST(test_mt_t1);
    RUN_TEST(test_mt_t_eq_n_cap);
    RUN_TEST(test_mt_t_gt_n);
    RUN_TEST(test_mt_rev_large);
    RUN_TEST(test_mt_const);
    RUN_TEST(test_mt_small_domain);
    RUN_TEST(test_mt_extremes);
    return UNITY_END();
}

