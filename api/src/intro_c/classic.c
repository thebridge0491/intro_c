#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <log4c.h>

static log4c_category_t *prac1;

static bool is_less(int a, int b) { return a < b; }
static bool is_grtr(int a, int b) { return a > b; }

static long add_a_b(const long a, const long b) { return a + b; }
static long mul_a_b(const long a, const long b) { return a * b; }

static float expt_iter(const float b, const float n, const float acc) {
    return n > 0.0f ? expt_iter(b, n - 1, acc * b) : acc;
}
float expt_i(const float b, const float n) {return expt_iter(b, n, 1.0f);}

float expt_lp(const float b, const float n) {
    float acc = 1.0f;
    for (float i = n; 0.0f < i; --i)
        acc *= b;
    return acc;
}

float square_i(const float n) { return expt_i(n, 2.0);   // powf(n, 2.0f);
}

float square_lp(const float n) { return expt_lp(n, 2.0);
}

static float fast_expt_iter(const float b, const int n, const float acc) {
    if (0 >= n) {
        return acc;
    } else if (0 == (n % 2)) {
        return fast_expt_iter(b, n - 2, acc * b * b);
    } else {
        return fast_expt_iter(b, n - 1, acc * b);
    }
}
float fast_expt_i(const float b, const float n) {
    return fast_expt_iter(b, n, 1.0);
}

float fast_expt_lp(const float b, const float n) {
    float acc = 1.0f;
    for (int i = n; 0.0f < i; ) {
        if (0 == (i % 2)) {
            acc *= (b * b);
            i -= 2.0f;
        } else {
            acc *= b;
            i -= 1.0f;
        }
    }
    return acc;
}

static long numseq_math_iter(long (*op)(long a, long b), const long acc,
        const long hi, const long lo) {
    return hi >= lo ? numseq_math_iter(op, op(acc, lo), hi, lo + 1) : acc;
}
static long numseq_math_i(long (*op)(long a, long b), const long init,
        const long hi, const long lo) {
    return numseq_math_iter(op, init, hi, lo);
}

static long numseq_math_lp(long (*op)(long a, long b), const long init,
        const long hi, const long lo) {
    long acc = init;
    for (long i = lo; hi >= i; ++i)
        acc = op(acc, i);
    return acc;
}

long sum_to_i(const long hi, const long lo) {
    return numseq_math_i(add_a_b, 0L, hi, lo);
}

long sum_to_lp(const long hi, const long lo) {
    return numseq_math_lp(add_a_b, 0L, hi, lo);
}

long fact_i(const long n) {
    prac1 = log4c_category_get("prac");
    log4c_category_log(prac1, LOG4C_PRIORITY_INFO, "fact_i");
    return numseq_math_i(mul_a_b, 1L, n, 1L);
}

long fact_lp(const long n) {
    return numseq_math_lp(mul_a_b, 1L, n, 1L);
}

static int fib_iter(const int s0, const int s1, const int cnt) {
    return 0 >= cnt ? s0 : fib_iter(s1, s0 + s1, cnt - 1);
}
int fib_i(const int n) {
    return fib_iter(0, 1, n);
}

int fib_lp(const int n) {
    int acc = 0;
    for (int sum0 = 0, sum1 = 1, ct = n; 0 <= ct; --ct) {
        acc = sum0;
        sum0 = sum1;
        sum1 = sum1 + acc;
    }
    return acc;
}

static void mk_rows(const int n, int **arr2d) {
    for (int row = 1; n >= row; ++row) {
        arr2d[row][0] = arr2d[row][row] = 1;
        for (int col = 1; row > col; ++col)
            arr2d[row][col] = arr2d[row - 1][col - 1] + arr2d[row - 1][col];
    }
}

int** pascaltri_add(const int n) {
    int **result = NULL;
    if (NULL == (result = calloc(n + 1, sizeof(int*)))) {
        perror("calloc error(arr2d)");
        return NULL;
	}
    for (int row = 0; n >= row; ++row)
        for (int col = 0; row >= col; ++col)
            if (NULL == (result[row] = calloc(row + 1, sizeof(int)))) {
                perror("calloc error(arr2d[i])");
                return NULL;
			}
    result[0][0] = 1;
    mk_rows(n, result);
    return result;    
}

void print_pascaltri(const int n, const int **arr2d) {
    if (NULL == arr2d)
        return;
    for (int row = 0; n >= row; ++row, putchar('\n'))
        for (int col = 0; row >= col; ++col)
            printf("%3i ", arr2d[row][col]);
    putchar('\n');
}

int quot_m(const int n, const int d) {return n / d;}

int rem_m(const int n, const int d) {return n % d;} //n - (d * quot_m(n, d));}

static int euclid_iter(const int a, const int b) {
    return 0 == b ? a : euclid_iter(b, a % b);
}
static int euclid_i(const int m, const int n) {
    return euclid_iter(m, n);
}

static int euclid_lp(const int m, const int n) {
    int acc = m;
    for (int b = n, swap = 0; 0 < b; ) {
        swap = acc;
        acc = b;
        b = swap % b;
    }
    return acc;
}

static int gcd_iter(const int acc, const int idx, const int *arr) {
    return 0 > idx ? acc : gcd_iter(euclid_i(acc, arr[idx]), idx - 1, arr);
}
int gcd_i(const int nelts, const int *arr) {
    return 0 > nelts ? 1 : gcd_iter(arr[0], nelts - 1, arr);
}

int gcd_lp(const int nelts, const int *arr) {
    int acc = (2 <= nelts) ? euclid_lp(arr[0], arr[1]) :
        ((1 == nelts) ? arr[0] : 1);
    for (int i = 2; nelts > i; ++i)
        acc = euclid_lp(acc, arr[i]);
    return acc;
}

static int lcm_iter(const int acc, const int idx, const int *arr) {
    return 0 > idx ? acc : lcm_iter(acc * arr[idx] / euclid_i(acc, arr[idx]), 
        idx - 1, arr);
}
int lcm_i(const int nelts, const int *arr) {
    return 0 > nelts ? 1 : lcm_iter(1, nelts - 1, arr);
}

int lcm_lp(const int nelts, const int *arr) {
    int acc = (2 <= nelts) ? (arr[0] * arr[1]) / euclid_lp(arr[0], arr[1]) :
        ((1 == nelts) ? arr[0] : 1);
    for (int i = 2; nelts > i; ++i)
        acc = (acc * arr[i]) / euclid_lp(acc, arr[i]);
    return acc;
}

static int* base_expand_iter(const int b, const int n, const int idx,
        int *arr) {
    if (0 <= idx) {
        arr[idx] = n % b;
        return base_expand_iter(b, n / b, idx - 1, arr);
    }
    return arr;
}
int* base_expand_i(const int b, const int n) {
    int *res = NULL, len_arr = 0;
    for (int i = n; 0 < i; i = i / b)
        len_arr += 1;
    if (NULL == (res = malloc(len_arr * sizeof(int)))) {
        perror("malloc error");
        return NULL;
	}
    return base_expand_iter(b, n, len_arr - 1, res);
}

int* base_expand_lp(const int b, const int n) {
    int *res = NULL, len_arr = 0;
    for (int i = n; 0 < i; i = i / b)
        len_arr += 1;
    if (NULL == (res = malloc(len_arr * sizeof(int)))) {
        perror("malloc error");
        return NULL;
	}
    for (int i = n, j = len_arr - 1; 0 < i; i = i / b, --j)
        res[j] = i % b;
    return res;
}

static int base_to10_iter(const int acc, const int b, const int idx, 
        const int len_arr, const int *arr) {
    return len_arr <= idx ? acc : 
        base_to10_iter(acc + arr[idx] * powf(b, (len_arr - idx - 1)), 
        b, idx + 1, len_arr, arr);
}
int base_to10_i(const int b, const int len_arr, const int *arr) {
    return base_to10_iter(0, b, 0, len_arr, arr);
}

int base_to10_lp(const int b, const int len_arr, const int *arr) {
    int acc = 0;
    for (int i = len_arr - 1, ct = 0; 0 <= i; --i, ++ct)
        acc += arr[i] * powf(b, ct);
    return acc;
}

static int* range_step_iter(const int step, int start, const int stop,
        const int idx, const int len_arr, int *arr) {
    if (len_arr > idx) {
        arr[idx] = start;
        return range_step_iter(step, start + step, stop, idx + 1, len_arr, arr);
    }
    return arr;
}
int* range_step_i(const int step, int start, const int stop) {
    int *res = NULL, len_arr = 0;
    bool (*rel_op)(int a, int b) = (step > 0) ? is_grtr : is_less;
    for (int i = start; rel_op(stop, i); i = i + step)
        len_arr += 1;
    if (NULL == (res = malloc(len_arr * sizeof(int)))) {
        perror("malloc error");
        return NULL;
	}
    return range_step_iter(step, start, stop, 0, len_arr, res);
}

int* range_step_lp(const int step, int start, const int stop) {
    int *res = NULL, len_arr = 0;
    bool (*rel_op)(int a, int b) = (step > 0) ? is_grtr : is_less;
    for (int i = start; rel_op(stop, i); i = i + step)
        len_arr += 1;
    if (NULL == (res = malloc(len_arr * sizeof(int)))) {
        perror("malloc error");
        return NULL;
	}
    for (int i = start, j = 0; rel_op(stop, i); i = i + step, ++j)
        res[j] = i;
    return res;
}

int* range_i(const int start, const int stop) {
    return range_step_i(1, start, stop);
}

int* range_lp(const int start, const int stop) {
    return range_step_lp(1, start, stop);
}
