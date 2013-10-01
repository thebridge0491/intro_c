#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <check.h>

#include "intro_c/classic_asm.h"
#include "intro_c/classic_f90.h"
#include "intro_c/classic_c.h"

static bool in_epsilon(float tolerance, const void *a, const void *b) {
	float x = *(float*)a, y = *(float*)b, delta = fabs(tolerance);
	//return (x - delta) <= y && (x + delta) >= y;
	return !((x + delta) < y) && !((y + delta) < x);
}

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
    unsigned long (*funcs[])(unsigned int n) = {fact_asm_i, fact_asm_lp,
		fact_f90_i, fact_f90_lp, fact_i, fact_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i)
        ck_assert_int_eq(120L, funcs[i](5));
} END_TEST

START_TEST (test_expt) {
    float (*funcs[])(float b, float n) = {expt_asm_i, expt_asm_lp, expt_f90_i, 
    	expt_f90_lp, expt_i, expt_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    float exp = powf(11.0f, 6.0f);
    
    for (int i = 0; len_funcs > i; ++i) {
    	float res = funcs[i](11.0f, 6.0f);
        ck_assert_msg(in_epsilon(0.001f * exp, &exp, &res), 
			"exp(%.1f) : res(%.1f)", exp, res);
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
