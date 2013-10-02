#include <log4c.h>

static log4c_category_t *prac1;

static long fact_iter(const int n, const long acc) {
    return n > 1 ? fact_iter(n - 1, acc * n) : acc;
}
long fact_i(const int n) {
    prac1 = log4c_category_get("prac");
    log4c_category_log(prac1, LOG4C_PRIORITY_INFO, "fact_i");
    return fact_iter(n, 1L);
}

long fact_lp(const int n) {
    long acc = 1;
    for (int i = n; i > 1; --i)
        acc *= i;
    return acc;
}

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
