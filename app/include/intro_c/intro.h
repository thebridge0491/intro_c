#ifndef INTRO_H
#define INTRO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <glib.h>

#include <regex.h>
#include <pcre.h>


#if (defined __APPLE__)
#include <sys/time.h>

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 0
#endif

int clock_gettime(clockid_t clock_id, struct timespec *tp);
#endif

void str_append(char **dest, const char *src);
const char* string_replicate(const int n, const char *str, const int len_buf,
    const char *buf);

void chk_strtol(int32_t *ret, const char *nptr, char **endptr, int32_t base);

void get_line(char *buf, int buf_sz, FILE *fIn, bool use_fgets);

char* inicfg_to_str(GKeyFile *cfg_ini);

int int_cmp(const void *a, const void *b);
int float_cmp(float tolerance, const void *a, const void *b);
bool in_epsilon(float tolerance, const void *a, const void *b);

int compile_regex(regex_t *r, const char *regex_txt, int cflags);
int match_regex(regex_t *r, const char *to_match);

pcre* compile_pcre(const char *pat, int cflags);
int match_pcre(pcre *re, int *ovector, const char *txt);

void cartesian_prod_ints(const int arr1[], const int len_arr1, 
    const int arr2[], const int len_arr2, int prod_arr[][2]);
void cartesian_prod_floats(const float arr1[], const int len_arr1, 
    const float arr2[], const int len_arr2, float prod_arr[][2]); 

#ifdef __cplusplus
}
#endif

#endif // INTRO_H
