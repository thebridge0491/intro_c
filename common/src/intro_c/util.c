#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <math.h>
#include <float.h>

#include "intro_c/util.h"


#define MAX_ERROR_MSG 80
#define OVECCOUNT 30

#if (defined __APPLE__)

int clock_gettime(clockid_t clock_id, struct timespec* tp) {
	struct timeval tn;
	int res;
	
	if ((res = gettimeofday(&tn, NULL)))
		return res;
	tp->tv_sec = tn.tv_sec;
	tp->tv_nsec = tn.tv_usec * 1000;
	return 0;
}
#endif


void str_append(char **dest, const char *src) {
    char *old_dest = *dest, *result;
    
    result = (char*)calloc(strlen(*dest) + strlen(src) + 1, sizeof(char));
    strncpy(result, old_dest, 1 + strlen(old_dest));
    strncat(result, src, strlen(src));
    free(old_dest);
    *dest = result;
}
const char* string_replicate(const int n, const char *str, 
        const int len_buf, const char *buf) {
    const int len_str = strlen(str);
    char *p = (char*)buf;
    
    for (int i = 0, j = 0; n > i && (len_buf - 1) > (j + len_str); ++i,
            p += len_str, j += len_str) {
        memcpy(p, str, len_str);
    }
    *p = '\0';
    return buf;
}
void chk_strtol(int32_t *ret, const char *nptr, char **endptr, int32_t base) {
    int32_t errno_reset = errno;
    errno = 0;
    *ret = strtol(nptr, endptr, base);

    if ((ERANGE == errno && (INT32_MAX == *ret || INT32_MIN == *ret)) 
            || (0 != errno && 0 == *ret)) {
        perror("Error: strtol");
        exit(EXIT_FAILURE);
	}
    errno = errno_reset;
}
void get_line(char *buf, int buf_sz, FILE *fIn, bool use_fgets) {
    bool is_done;
    
    if (use_fgets)
        is_done = (NULL == fgets(buf, buf_sz, fIn));
    else
        is_done = (EOF == fscanf(fIn, "%[^\n]", buf)); 
    if (is_done) {
        perror("get_line");
        exit(EXIT_FAILURE);
    } 
    if ('\n' == buf[strlen(buf) - 1])
        buf[strlen(buf) - 1] = '\0';
}

char* inicfg_to_str(GKeyFile *cfg_ini) {
    const char *beg = "{", *sep = ", ", *stop = "}\n";
    char *result = (char*)calloc(1, sizeof(char)), buf[32];
    gsize group_len, sect_len;
    GError *error = NULL;
    
    str_append(&result, beg);
    
    gchar **groups = g_key_file_get_groups(cfg_ini, &group_len);
    for (int i = 0; group_len > i; ++i) {
        gchar **sects = g_key_file_get_keys(cfg_ini, groups[i], &sect_len, 
			&error);
        for (int j = 0; sect_len > j; ++j) {
			gchar *value = g_key_file_get_string(cfg_ini, groups[i], sects[j],
                &error);
            snprintf(buf, 32, "%s(%s:%s => %s)", (0 == i) ? "" : sep, 
                groups[i], sects[j], value);
            str_append(&result, buf);
            g_free(value);
            if (NULL != error)
                g_error_free(error);
        }
        g_strfreev(sects);
    }
    g_strfreev(groups);
    str_append(&result, stop);
    return result;
}

int int_cmp(const void *a, const void *b) {
    int x = *(int*)a, y = *(int*)b;
    return (x == y) ? 0 : ((x < y) ? -1 : 1);
}
int float_cmp(float tolerance, const void *a, const void *b) {
    float x = *(float*)a, y = *(float*)b, delta = fabs(tolerance);
    return ((x + delta) < y) ? -1 : ((x - delta) > y) ? 1 : 0;
}
bool in_epsilon(float tolerance, const void *a, const void *b) {
	float x = *(float*)a, y = *(float*)b, delta = fabs(tolerance);
	//return (x - delta) <= y && (x + delta) >= y;
	return !((x + delta) < y) && !((y + delta) < x);
}


int compile_regex(regex_t *r, const char *regex_txt, int cflags) {
    int status = regcomp(r, regex_txt, cflags);
    if (0 == status)
        return 0;
    char error_msg[MAX_ERROR_MSG];
    regerror(status, r, error_msg, MAX_ERROR_MSG);
    fprintf(stderr, "Regex error compiling '%s': %s\n", regex_txt, error_msg);
    return 1;
}
int match_regex(regex_t *r, const char *to_match) {
    const char *p = to_match;
    const int n_matches = 10;
    regmatch_t matches[n_matches];
    int nomatch = regexec(r, p, n_matches, matches, 0);
    return (nomatch) ? nomatch : 0;
}

pcre* compile_pcre(const char *pat, int cflags) {
    const char *error;
    int erroffset;
    pcre *re = pcre_compile(pat, cflags, &error, &erroffset, NULL);
    if (NULL != re)
        return re;
    fprintf(stderr, "PCRE compile error at offset %i: %s\n", erroffset, error);
    return NULL;
}
int match_pcre(pcre *re, int *ovector, const char *txt) {
    int str_len = (int)strlen(txt);
    return pcre_exec(re, NULL, txt, str_len, 0, 0, ovector, OVECCOUNT);
}


void cartesian_prod_ints(const int arr1[], const int len_arr1, 
        const int arr2[], const int len_arr2, int arr_prod[][2]) {
    for (int i = 0, idxX = 0; len_arr1 > i; ++i) {
        for (int j = 0; len_arr2 > j; ++j) {
            idxX = j + (i * len_arr2);
            arr_prod[idxX][0] = arr1[i]; arr_prod[idxX][1] = arr2[j];
        }
    }
}
void cartesian_prod_floats(const float arr1[], const int len_arr1, 
        const float arr2[], const int len_arr2, float arr_prod[][2]) {
    for (int i = 0, idxX = 0; len_arr1 > i; ++i) {
        for (int j = 0; len_arr2 > j; ++j) {
            idxX = j + (i * len_arr2);
            arr_prod[idxX][0] = arr1[i]; arr_prod[idxX][1] = arr2[j];
        }
    }
}


int intptr_cmp(const void *a, const void *b) {
    return int_cmp(*(int**)a, *(int**)b);
}
int floatptr_cmp(float tolerance, const void *a, const void *b) {
    return float_cmp(tolerance, *(float**)a, *(float**)b);
}

static int intptr_fmt(char *str, size_t size, const void *elem) {
    return snprintf(str, size, "%i,", *(int*)elem);
}
static int floatptr_fmt(char *str, size_t size, const void *elem) {
    return snprintf(str, size, "%.2f,", *(float*)elem);
}
static int strcharptr_kv_fmt(char *str, size_t size, const void *key, 
        const void *val) {
    return snprintf(str, size, "(%s => %c),", (char*)key, *(char*)val);
}
static void ptrel_to_str(int (*el_fmt)(char *str, size_t size, 
        const void *elem), const void *elem, char **dest) {
    int buf_len = 31;
    char buf[buf_len + 1];
    el_fmt(buf, buf_len, elem);
    str_append(dest, buf);
}
static void ptrkv_to_str(int (*kv_fmt)(char *str, size_t size, 
        const void *key, const void *val), const void *key, const void *val, 
        char **dest) {
    int buf_len = 31;
    char buf[buf_len + 1];
    kv_fmt(buf, buf_len, key, val);
    str_append(dest, buf);
}
void intptr_el_to_str(const void *elem, char **dest) {
    ptrel_to_str(intptr_fmt, elem, dest);
}
void floatptr_el_to_str(const void *elem, char **dest) {
    ptrel_to_str(floatptr_fmt, elem, dest);
}
void strcharptr_kv_to_str_void(const void *key, const void *val, char **dest) {
    ptrkv_to_str(strcharptr_kv_fmt, key, val, dest);
}
bool strcharptr_kv_to_str_bool(const void *key, const void *val, char **dest) {
    strcharptr_kv_to_str_void(key, val, dest);
    return 0;
}

void mkstring_ptrarray(void (*el_to_str)(const void *elem, char **dest), 
        int nelts, const void **arr, char **dest) {
    const char *beg = "[", *end = "]";
    str_append(dest, beg);
    for (int i = 0; nelts > i; ++i)
        el_to_str(arr[i], dest);
    str_append(dest, end);
}
void mkstring_ints(int nelts, const int *arr, char **dest) {
    const char *beg = "[", *end = "]";
    str_append(dest, beg);
    for (int i = 0; nelts > i; ++i)
        intptr_el_to_str(&arr[i], dest);
    str_append(dest, end);
}
void mkstring_floats(int nelts, const float *arr, char **dest) {
    const char *beg = "[", *end = "]";
    str_append(dest, beg);
    for (int i = 0; nelts > i; ++i)
        floatptr_el_to_str(&arr[i], dest);
    str_append(dest, end);
}
