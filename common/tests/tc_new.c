#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <check.h>

#include "intro_c/util.h"

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

START_TEST (test_1) {
    ck_assert_int_eq(2 * 2, 4);
} END_TEST

START_TEST (test_dbl) {
    float exp = 4.0, res = 4.0;
    ck_assert_msg(in_epsilon(0.001f * exp, &exp, &res),
		"msg: exp(%.1f), res(%.1f)", exp, res);
} END_TEST

START_TEST (test_str) {
    char *str1 = "Hello";
    ck_assert_str_eq(str1, "Hello");
} END_TEST

START_TEST (test_bad) {
    ck_assert_int_eq(2 * 2, initNum2);
} END_TEST

START_TEST (test_fail) {
    ck_abort_msg(2 * 2 == initNum1, "Fail. %i", initNum1);
} END_TEST


Suite* tc_new_suite(void) {
    Suite *suite = suite_create("tc_new");
    TCase *tc_new = tcase_create("Tc_new");
    tcase_set_tags(tc_new, "Tag1");
    
    tcase_add_unchecked_fixture(tc_new, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_new, setupTestA, teardownTestA);
    
    TFun tests[] = {
		test_1, test_dbl, test_str, test_bad, test_fail
	};
    int arrLen = sizeof(tests) / sizeof(tests[0]);
    
    /* Add the tests to the tcase */
    for (int i = 0; arrLen > i; ++i)
        tcase_add_test(tc_new, tests[i]);
    //tcase_add_test(tc_new, test_method);
    
    /* Add tcase to suite */
    suite_add_tcase(suite, tc_new);
    return suite;
}
