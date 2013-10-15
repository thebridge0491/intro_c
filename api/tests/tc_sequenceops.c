#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <check.h>

#include "intro_c/util.h"
#include "intro_c/sequenceops.h"

static int initNum1 = 0, initNum2 = 0;

static void setupTCase(void) {
    printf("###setup TCase###\n");
    initNum1 = 5;
    return;
}

static void teardownTCase(void) {
    initNum1 = 0;
    printf("###teardown TCase###\n\n");
    return;
}

static void setupTestA(void) {
    printf("setup Test...\n");
    initNum2 = -1;
    return;
}

static void teardownTestA(void) {
    initNum2 = 0;
    printf("...teardown Test\n");
    return;
}

static int n0 = 0, n1 = 1, n2 = 2, n3 = 3, n4 = 4;
static const void *ptr_ints[] = {&n0, &n1, &n2, &n3, &n4};
static const void *ptr_ints_rev[] = {&n4, &n3, &n2, &n1, &n0};
static int len_ints = sizeof(ptr_ints) / sizeof(ptr_ints[0]);
static int add1(const int idx) { return idx; }
static int sub1(const int idx) { return 4 - idx;}

START_TEST (test_tabulate) {
    int* (*funcs[])(int (*func)(const int idx), const int cnt) = {
        tabulate_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        int *res = funcs[i](add1, 5), *res_rev = funcs[i](sub1, 5);
        ck_assert(NULL != res);
        ck_assert(NULL != res_rev);
        
        for (int j = 0; len_ints > j; ++j) {
            ck_assert_msg(0 == int_cmp(ptr_ints[j], &res[j]), 
                "exp(%i) : res(%i)", *(int*)ptr_ints[j], res[j]);
            ck_assert_msg(0 == int_cmp(ptr_ints_rev[j], &res_rev[j]), 
                "exp_rev(%i) : res_rev(%i)", *(int*)ptr_ints_rev[j],
                res_rev[j]);
        }
        free(res);
        free(res_rev);
    }
} END_TEST

START_TEST (test_index_of) {
    int (*funcs[])(const void *data, int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) = {index_of_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    int el = 3;
    for (int i = 0; len_funcs > i; ++i) {
        ck_assert_int_eq(3, funcs[i](&el, len_ints, ptr_ints, int_cmp));
        ck_assert_int_eq(1, funcs[i](&el, len_ints, ptr_ints_rev, int_cmp));
    }
} END_TEST

START_TEST (test_find) {
    void* (*funcs[])(const void *data, int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) = {find_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    int el = 3;
    for (int i = 0; len_funcs > i; ++i) {
        //ck_assert(0 == int_cmp(&el,funcs[i](&el,len_ints,ptr_ints,int_cmp)));
        ck_assert_int_eq(el, *(int*)funcs[i](&el, len_ints, ptr_ints,
            int_cmp));
        ck_assert_int_eq(el, *(int*)funcs[i](&el, len_ints, ptr_ints_rev, 
            int_cmp));
    }
} END_TEST

START_TEST (test_minmax) {
    void (*funcs[])(const void* minMaxArr[2], int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) = {minmax_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    int lo = 0, hi = 4;
    const void* minMaxArr[2];
    for (int i = 0; len_funcs > i; ++i) {
        minmax_lp(minMaxArr, len_ints, ptr_ints, int_cmp);
        //ck_assert(0 == int_cmp(&lo, minMaxArr[0]));
        ck_assert_int_eq(lo, *(int*)minMaxArr[0]);
        ck_assert_int_eq(hi, *(int*)minMaxArr[1]);
        minmax_lp(minMaxArr, len_ints, ptr_ints_rev, int_cmp);
        ck_assert_int_eq(lo, *(int*)minMaxArr[0]);
        ck_assert_int_eq(hi, *(int*)minMaxArr[1]);
    }
} END_TEST

START_TEST (test_reverse) {
    void (*funcs[])(int nelts, void **arr) = {reverse_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        void **ptr_tmps = copy_of(len_ints, ptr_ints);
        funcs[i](len_ints, ptr_tmps);
        
        for (int j = 0; len_ints > j; ++j)
            //ck_assert(0 == int_cmp(ptr_ints_rev[j], ptr_tmps[j]));
            ck_assert_int_eq(*(int*)ptr_ints_rev[j], *(int*)ptr_tmps[j]);
        free(ptr_tmps);
    }
} END_TEST

START_TEST (test_copy_of) {
    void** (*funcs[])(int nelts, const void **arr) = {copy_of};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        void **ptr_tmps = funcs[i](len_ints, ptr_ints);
        
        for (int j = 0; len_ints > j; ++j)
            //ck_assert(0 == int_cmp(ptr_ints_rev[j], ptr_tmps[j]));
            ck_assert_int_eq(*(int*)ptr_ints[j], *(int*)ptr_tmps[j]);
        free(ptr_tmps);
    }
} END_TEST

static void sum_el_acc(void *el, void *acc) {
    *(int*)acc = *(int*)el + *(int*)acc;
}

START_TEST (test_foreach) {
    void (*funcs[])(void *acc, void (*func)(void *el, void *acc), int nelts,
        const void **arr) = {foreach_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        int res = 0;
        funcs[i](&res, sum_el_acc, len_ints, ptr_ints);
        ck_assert_int_eq(10, res);
        res = 0;
        funcs[i](&res, sum_el_acc, len_ints, ptr_ints_rev);
        ck_assert_int_eq(10, res);
    }
} END_TEST

START_TEST (test_sort) {
    void (*funcs[])(void** arr, int lo, int hi, 
		int (*cmp)(const void* a, const void* b)) = {quick_sort};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        void **exp_tmps = copy_of(len_ints, ptr_ints_rev);
        qsort(exp_tmps, len_ints, sizeof(exp_tmps[0]), intptr_cmp);
        void **res_tmps = copy_of(len_ints, ptr_ints_rev);
        funcs[i](res_tmps, 0, len_ints - 1, int_cmp);
        
        for (int j = 0; len_ints > j; ++j)
            //ck_assert(0 == int_cmp(exp_tmps[j], res_tmps[j]));
            ck_assert_int_eq(*(int*)exp_tmps[j], *(int*)res_tmps[j]);
        ck_assert(is_ordered_lp(len_ints, (const void**)res_tmps, int_cmp));
        free(exp_tmps);
        free(res_tmps);
    }
} END_TEST

START_TEST (test_misc) {
    int len = 5, hd = 0, hd_rev = 4, n = 2, nth2 = 1, nth2_rev = 3;
    ck_assert_int_eq(len, sizeof(ptr_ints) / sizeof(ptr_ints[0]));
    ck_assert_int_eq(len, sizeof(ptr_ints) / sizeof(ptr_ints_rev[0]));
    
    //ck_assert(0 == int_cmp(&hd, ptr_ints[0]));
    ck_assert_int_eq(hd, *(int*)ptr_ints[0]);
    ck_assert_int_eq(hd_rev, *(int*)ptr_ints_rev[0]);
    
    //ck_assert(0 == int_cmp(&nth2, ptr_ints[n - 1]));
    ck_assert_int_eq(nth2, *(int*)ptr_ints[n - 1]);
    ck_assert_int_eq(nth2_rev, *(int*)ptr_ints_rev[n - 1]);
} END_TEST

START_TEST (test_append) {
    void (*funcs[])(int nelts1, void ***arr1, int nelts2, const void **arr2) =
        {append_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        void **ptr_tmps = copy_of(len_ints, ptr_ints);
        funcs[i](len_ints, &ptr_tmps, len_ints, ptr_ints_rev);
        for (int j = 0; len_ints > j; ++j)
            //ck_assert(0 == int_cmp(ptr_ints[j], ptr_tmps[j]));
            ck_assert_int_eq(*(int*)ptr_ints[j], *(int*)ptr_tmps[j]);
        for (int j = len_ints, k = 0; len_ints > k; ++j, ++k)
            ck_assert_int_eq(*(int*)ptr_ints_rev[k], *(int*)ptr_tmps[j]);
        free(ptr_tmps);
    }
} END_TEST

START_TEST (test_interleave) {
    void** (*funcs[])(const int nelts1, const void **arr1, const int nelts2,
        const void **arr2) = {interleave_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    int n0 = 0, n1 = 1, n2 = 2, n3 = 3, n4 = 4;
    const void *ptr_ints[] = {&n0, &n1, &n2, &n3, &n4};
    const void *ptr_ints_rev[] = {&n4, &n3, &n2, &n1, &n0};
    int len_ints = sizeof(ptr_ints) / sizeof(ptr_ints[0]);
    
    const void *exp[] = {&n0, &n4, &n1, &n3, &n2, &n2, &n3, &n1, &n4, &n0};
    for (int i = 0; len_funcs > i; ++i) {
        void **res = funcs[i](len_ints, ptr_ints, len_ints, ptr_ints_rev);
        ck_assert(NULL != res);
        
        for (int j = 0; (len_ints + len_ints) > j; ++j)
            ck_assert_msg(0 == int_cmp(exp[j], res[j]), "exp(%i) : res(%i)", 
                *(int*)exp[j], *(int*)res[j]);
        free(res);
    }
} END_TEST

START_TEST (test_add_last) {
    void (*funcs[])(const void *data, int nelts, void ***arr) = {add_last_m};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        int el = 51;
        void **ptr_tmps = copy_of(len_ints, ptr_ints);
        funcs[i](&el, len_ints, &ptr_tmps);
        
        for (int j = 0; len_ints > j; ++j)
            //ck_assert(0 == int_cmp(ptr_ints[j], ptr_tmps[j]));
            ck_assert_int_eq(*(int*)ptr_ints[j], *(int*)ptr_tmps[j]);
        ck_assert_int_eq(el, *(int*)ptr_tmps[len_ints]);
        free(ptr_tmps);
    }
} END_TEST


Suite* tc_sequenceops_suite(void) {
    Suite *suite = suite_create("tc_sequenceops");
    TCase *tc_sequenceops = tcase_create("Tc_sequenceops");
    tcase_set_tags(tc_sequenceops, "Tag1");
    
    tcase_add_unchecked_fixture(tc_sequenceops, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_sequenceops, setupTestA, teardownTestA);
    
    TFun tests[] = {
		test_tabulate, test_index_of, test_find, test_minmax, test_reverse, 
		test_copy_of, test_foreach, test_sort, test_misc, test_append, 
		test_interleave, test_add_last
	};
    int arrLen = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; arrLen > i; ++i)
        tcase_add_test(tc_sequenceops, tests[i]);
    
    suite_add_tcase(suite, tc_sequenceops);
    return suite;
}
