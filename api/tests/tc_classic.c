#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <check.h>

#include "intro_c/util.h"
#include "intro_c/classic.h"

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

START_TEST (test_fact) {
    long (*funcs[])(int n) = {fact_i, fact_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i)
        ck_assert_int_eq(120, funcs[i](5));
} END_TEST

START_TEST (test_expt) {
    float (*funcs[])(float b, float n) = {expt_i, expt_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    float param1[] = {2.0f, 11.0f, 20.0f}, param2[] = {3.0f, 6.0f, 10.0f};
    
    int len_param1 = sizeof(param1) / sizeof(param1[0]);
    int len_param2 = sizeof(param2) / sizeof(param2[0]);
    int len_params = len_param1 * len_param2;
    float prod_params[len_params][2];
    
    cartesian_prod_floats(param1, len_param1, param2, len_param2, prod_params);
    
    for (int i = 0; len_funcs > i; ++i) {
        for (int j = 0; len_params > j; ++j) {
            float exp = powf(prod_params[j][0], prod_params[j][1]);
            float res = funcs[i](prod_params[j][0], prod_params[j][1]);
            ck_assert_msg(in_epsilon(0.001f * exp, &exp, &res),
				"exp(%.1f) : res(%.1f)", exp, res);
        }
    }
} END_TEST


Suite* tc_classic_suite(void) {
    Suite *suite = suite_create("tc_classic");
    TCase *tc_classic = tcase_create("Tc_classic");
    tcase_set_tags(tc_classic, "Tag1");
    
    tcase_add_unchecked_fixture(tc_classic, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_classic, setupTestA, teardownTestA);
    
    TFun tests[] = {
		test_fact, test_expt
	};
    int arrLen = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; arrLen > i; ++i)
        tcase_add_test(tc_classic, tests[i]);
    
    suite_add_tcase(suite, tc_classic);
    return suite;
}
