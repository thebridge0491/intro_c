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

START_TEST (test_square) {
    float (*funcs[])(const float n) = {square_i, square_lp};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    float floats1[] = {2.0f, 11.0f, 20.0f};
    for (int i = 0; len_funcs > i; ++i) {
        for (int j = 0; 3 > j; ++j) {
            float exp = powf(floats1[j], 2.0f), res = funcs[i](floats1[j]);
            ck_assert_msg(in_epsilon(0.001f * exp, &exp, &res), 
                "msg: exp(%.1f), res(%.1f)", exp, res);
        }
    }
} END_TEST

START_TEST (test_expt) {
    float (*funcs[])(float b, float n) = {expt_lp, expt_i, fast_expt_lp,
        fast_expt_i};
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

START_TEST (test_sum_to) {
    int (*funcs[])(const int hi, const int lo) = {sum_to_lp, sum_to_i};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        int exp = 15, res = funcs[i](5, 0);
        ck_assert_msg(0 == int_cmp(&exp, &res), "msg: exp(%i), res(%i)", 
            exp, res);
        exp = 75;
        res = funcs[i](15, 10);
        ck_assert_msg(0 == int_cmp(&exp, &res), "msg: exp(%i), res(%i)", 
            exp, res);
    }
} END_TEST

START_TEST (test_fact) {
    long (*funcs[])(int n) = {fact_lp, fact_i};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i)
        ck_assert_int_eq(120, funcs[i](5));
} END_TEST

START_TEST (test_fib) {
    int (*funcs[])(int n) = {fib_lp, fib_i};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    for (int i = 0; len_funcs > i; ++i)
        ck_assert_int_eq(13, funcs[i](7));
} END_TEST

START_TEST (test_quot_rem) {
    int (*quot_funcs[])(int n, int d) = {quot_m};
    int (*rem_funcs[])(int n, int d) = {rem_m};
    int len_funcs = sizeof(quot_funcs) / sizeof(quot_funcs[0]);
    
    int arg1[] = {10, -10}, arg2[] = {3, -3};
    
    int len_arg1 = sizeof(arg1) / sizeof(arg1[0]);
    int len_arg2 = sizeof(arg2) / sizeof(arg2[0]);
    int len_args = len_arg1 * len_arg2;
    int prod_args[len_args][2];
    
    cartesian_prod_ints(arg1, len_arg1, arg2, len_arg2, prod_args);
    
    for (int i = 0; len_funcs > i; ++i) {
        for (int j = 0; len_args > j; ++j) {
            int n = prod_args[j][0], d = prod_args[j][1];
            div_t exp = div(n, d);
            int res_quot = quot_funcs[i](n, d), res_rem = rem_funcs[i](n, d);
            ck_assert_msg(exp.quot == res_quot && exp.rem == res_rem, 
                "exp(div(%i, %i) : div_t{.quot(%i), .rem(%i)}) != \
                res(quot(%i) rem(%i))", n, d, exp.quot, exp.rem, 
                res_quot, res_rem);
        }
    }
} END_TEST

START_TEST (test_gcd_lcm) {
    int (*gcd_funcs[])(const int len_arr, const int *arr) = {gcd_lp, gcd_i};
    int (*lcm_funcs[])(const int len_arr, const int *arr) = {lcm_lp, lcm_i};
    int len_funcs = sizeof(gcd_funcs) / sizeof(gcd_funcs[0]);
    
    const int arr1[] = {24, 16}, arr2_gcd[] = {24, 16, 12}, 
        arr2_lcm[] = {24, 16, 32};
    int len_arr1 = sizeof(arr1) / sizeof(arr1[0]);
    int len_arr2 = sizeof(arr2_gcd) / sizeof(arr2_gcd[0]);
    int exp1_gcd = 8, exp1_lcm = 48, exp2_gcd = 4, exp2_lcm = 96;
    
    for (int i = 0; len_funcs > i; ++i) {
        ck_assert_int_eq(exp1_gcd, gcd_funcs[i](len_arr1, arr1));
        ck_assert_int_eq(exp1_lcm, lcm_funcs[i](len_arr1, arr1));
        
        ck_assert_int_eq(exp2_gcd, gcd_funcs[i](len_arr2, arr2_gcd));
        ck_assert_int_eq(exp2_lcm, lcm_funcs[i](len_arr2, arr2_lcm));
    }
} END_TEST

START_TEST (test_base_expand) {
    int* (*funcs[])(const int b, const int n) = {base_expand_lp, base_expand_i};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    const int exp1[] = {1, 0, 1, 1}, exp2[] = {1, 1, 0, 1}, b1 = 2, b2 = 4,
        n1 = 11, n2 = 81;
    int len_arr1 = sizeof(exp1) / sizeof(exp1[0]);
    int len_arr2 = sizeof(exp2) / sizeof(exp2[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        int *res = NULL;
        
        ck_assert(NULL != (res = funcs[i](b1, n1)));
        for (int j = 0; len_arr1 > j; ++j)
            ck_assert_int_eq(exp1[j], res[j]);
        free(res);
        
        ck_assert(NULL != (res = funcs[i](b2, n2)));
        for (int j = 0; len_arr2 > j; ++j)
            ck_assert_int_eq(exp2[j], res[j]);
        free(res);
    }
} END_TEST

START_TEST (test_base_to10) {
    int (*funcs[])(const int b, const int len_arr, const int *arr) = 
        {base_to10_lp, base_to10_i};
    int len_funcs = sizeof(funcs) / sizeof(funcs[0]);
    
    const int arr1[] = {1, 0, 1, 1}, arr2[] = {1, 1, 0, 1}, b1 = 2, b2 = 4,
        exp1 = 11, exp2 = 81;
    int len_arr1 = sizeof(arr1) / sizeof(arr1[0]);
    int len_arr2 = sizeof(arr2) / sizeof(arr2[0]);
    
    for (int i = 0; len_funcs > i; ++i) {
        ck_assert_int_eq(exp1, funcs[i](b1, len_arr1, arr1));
        
        ck_assert_int_eq(exp2, funcs[i](b2, len_arr2, arr2));
    }
} END_TEST

START_TEST (test_range) {
    int* (*rg_step_funcs[])(const int step, int start,
        const int stop) = {range_step_lp, range_step_i};
    int* (*rg_funcs[])(const int start, const int stop) = {range_lp, range_i};
    int len_funcs = sizeof(rg_step_funcs) / sizeof(rg_step_funcs[0]);
    
    int start = 0, stop = 5;
    for (int i = 0; len_funcs > i; ++i) {
        int *res = NULL;
        
        ck_assert(NULL != (res = rg_step_funcs[i](1, start, stop)));
        for (int j = 0, k = start; stop > k; ++j, k = k + 1)
            ck_assert_int_eq(k, res[j]);
        free(res);
        
        ck_assert(NULL != (res = rg_funcs[i](start, stop)));
        for (int j = 0, k = start; stop > k; ++j, ++k)
            ck_assert_int_eq(k, res[j]);
        free(res);
    }
} END_TEST


Suite* tc_classic_suite(void) {
    Suite *suite = suite_create("tc_classic");
    TCase *tc_classic = tcase_create("Tc_classic");
    tcase_set_tags(tc_classic, "Tag1");
    
    tcase_add_unchecked_fixture(tc_classic, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_classic, setupTestA, teardownTestA);
    
    TFun tests[] = {
		test_square, test_expt, test_sum_to, test_fact, test_fib, 
		test_quot_rem, test_gcd_lcm, test_base_expand, test_base_to10,
		test_range
	};
    int arrLen = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; arrLen > i; ++i)
        tcase_add_test(tc_classic, tests[i]);
    
    suite_add_tcase(suite, tc_classic);
    return suite;
}
