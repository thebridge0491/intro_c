#ifndef CLASSIC_H
#define CLASSIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "intro_c/classic_puzzles.h"

float expt_i(const float b, const float n);
float expt_lp(const float b, const float n);
float fast_expt_i(const float b, const float n);
float fast_expt_lp(const float b, const float n);

float square_i(const float n);
float square_lp(const float n);

long sum_to_i(const long hi, const long lo);
long sum_to_lp(const long hi, const long lo);

long fact_i(const long n);
long fact_lp(const long n);

int fib_i(const int n);
int fib_lp(const int n);

int** pascaltri_add(const int n);
void print_pascaltri(const int n, const int ** arr2d);

int quot_m(const int n, const int d);
int rem_m(const int n, const int d);

int gcd_i(const int len_arr, const int *arr);
int gcd_lp(const int len_arr, const int *arr);

int lcm_i(const int len_arr, const int *arr);
int lcm_lp(const int len_arr, const int *arr);

int* base_expand_i(const int b, const int n);
int* base_expand_lp(const int b, const int n);

int base_to10_i(const int b, const int len_arr, const int *arr);
int base_to10_lp(const int b, const int len_arr, const int *arr);

int* range_step_i(const int step, int start, const int stop);
int* range_step_lp(const int step, int start, const int stop);
int* range_i(const int start, const int stop);
int* range_lp(const int start, const int stop);

#ifdef __cplusplus
}
#endif

#endif // CLASSIC_H
