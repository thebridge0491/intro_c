#include <stdlib.h>
#include <check.h>

#include "tc_new.h"

Suite* (*suite_funcs[])(void) = {tc_new_suite};

int main(int argc, char **argv) {
    int number_failed;
    //SRunner *sr = srunner_create(tc_new_suite());
    SRunner *sr = srunner_create(NULL);
    
    int arrLen = sizeof(suite_funcs) / sizeof(suite_funcs[0]);
    
    /* Add the suites to the suite runner */
    for (int i = 0; arrLen > i; ++i)
        srunner_add_suite(sr, suite_funcs[i]());
    //srunner_add_suite(sr, tc_new_suite());
    
    srunner_set_log(sr, "ts_main.log");
    srunner_run_all(sr, CK_NORMAL);    // CK_[MINIMAL|NORMAL|VERBOSE|ENV]
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    exit((0 == number_failed) ? EXIT_SUCCESS : EXIT_FAILURE);
}
