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


Suite* tc_sequenceops_suite(void) {
    Suite *suite = suite_create("tc_sequenceops");
    TCase *tc_sequenceops = tcase_create("Tc_sequenceops");
    tcase_set_tags(tc_sequenceops, "Tag1");
    
    tcase_add_unchecked_fixture(tc_sequenceops, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_sequenceops, setupTestA, teardownTestA);
    
    TFun tests[] = {
		test_index_of, test_reverse
	};
    int arrLen = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; arrLen > i; ++i)
        tcase_add_test(tc_sequenceops, tests[i]);
    
    suite_add_tcase(suite, tc_sequenceops);
    return suite;
}
