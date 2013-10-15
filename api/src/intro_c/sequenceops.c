#include <stdlib.h>
#include <string.h>
#include <log4c.h>

#include "intro_c/sequenceops.h"

static log4c_category_t *prac1;

void swap_items(int a, int b, void **arr) {
    void *swap = arr[a]; arr[a] = arr[b]; arr[b] = swap;
}

int* tabulate_lp(int (*func)(int idx), const int cnt) {
    int *res = NULL;
    if (NULL == (res = malloc(cnt * sizeof(int)))) {
        perror("malloc error");
        return NULL;
	}
    for (int i = 0; cnt > i; ++i)
        res[i] = func(i);
        //res[i] = (void*)func((void*)&i);
    return res;
}

static void index_find_lp(const void* idxFndArr[2], const void *data,
        int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) {
    idxFndArr[0] = -1; idxFndArr[1] = NULL;
    for (int i = 0; nelts > i; ++i)
        if (0 == cmp(arr[i], data)) {
            idxFndArr[0] = i; idxFndArr[1] = (void*)arr[i];
            break;
        }
}

int index_of_lp(const void *data, int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) {
    const void* idxFndArr[2];
    index_find_lp(idxFndArr, data, nelts, arr, cmp);
    return (int)idxFndArr[0];
}

void* find_lp(const void *data, int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) {
    const void* idxFndArr[2];
    index_find_lp(idxFndArr, data, nelts, arr, cmp);
    return (void*)idxFndArr[1];
}

void minmax_lp(const void* minMaxArr[2], int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) {
    minMaxArr[0] = minMaxArr[1] = arr[0];
    for (int i = 0; nelts > i; ++i)
        if (-1 == cmp(arr[i], minMaxArr[0]))
            minMaxArr[0] = arr[i];
        else if (1 == cmp(arr[i], minMaxArr[1]))
            minMaxArr[1] = arr[i];
}

void reverse_lp(int nelts, void **arr) {
    prac1 = log4c_category_get("prac");
    log4c_category_log(prac1, LOG4C_PRIORITY_INFO, "reverse_lp");
    for (int i = 0, j = nelts - 1; j > i; ++i, --j)
        swap_items(i, j, arr);
}

void** copy_of(int nelts, const void **arr) {
	void **new_arr = NULL;
	
    if (NULL == (new_arr = malloc(nelts * sizeof(void*)))) {
        perror("malloc error");
        return NULL;
	}
    memmove(new_arr, arr, nelts * sizeof(void*));
    return new_arr;
}

void foreach_lp(void *acc, void (*func)(void *el, void *acc), int nelts,
        const void **arr) {
    for (int i = 0; nelts > i; ++i)
        func((void*)arr[i], acc);
}


bool is_ordered_lp(int nelts, const void **arr, 
        int (*cmp)(const void *a, const void *b)) {
    for (int i = 1; nelts > i; ++i)
        if (0 < cmp(arr[i - 1], arr[i]))
            return false;
    return true;
}

static int qpartition(void** arr, int lo, int hi, 
		int (*cmp)(const void* a, const void* b)) {  		// O(n)
	int lwr = lo, upr = hi;
	
	while (lwr < upr) {
		while (0 >= cmp(arr[lwr], arr[lo]) && lwr < upr)
			++lwr;
		while (0 < cmp(arr[upr], arr[lo]))
			--upr;
		if (lwr < upr)
			swap_items(lwr, upr, arr);
	}
	swap_items(lo, upr, arr);
	return upr;
}
void quick_sort(void** arr, int lo, int hi, 
		int (*cmp)(const void* a, const void* b)) {			// O(n^2)*
	if (hi > lo) {											//expect O(n lg n)
		int rNdx = (rand() % (hi - lo + 1)) + lo;       // random pivot
		swap_items(lo, rNdx, arr);
		int split = qpartition(arr, lo, hi, cmp);
		quick_sort(arr, lo, split - 1, cmp);
		quick_sort(arr, split + 1, hi, cmp);
	}
}


void append_lp(int nelts1, void ***arr1, int nelts2, const void **arr2) {
	void **new_arr = NULL;
	
    if (NULL == (new_arr = malloc((nelts1 + nelts2) * sizeof(void*)))) {
        perror("malloc error");
        return;
	}
    memcpy(new_arr, *arr1, nelts1 * sizeof(void*));
    //memcpy(new_arr + nelts1, arr2, nelts2 * sizeof(void*));
    for (int i = nelts1, j = 0; nelts2 > j; ++i, ++j)
        new_arr[i] = (void*)arr2[j];
    free(*arr1);
    *arr1 = new_arr;
}

void** interleave_lp(const int nelts1, const void **arr1, const int nelts2,
        const void **arr2) {
    void **res = NULL;
    
    if (NULL == (res = malloc((nelts1 + nelts2) * sizeof(void*)))) {
        perror("malloc error");
        return NULL;
	}
    for (int i = 0, j = 0, k = 0; nelts1 > i || nelts2 > j; ++i, ++j) {
        if (nelts1 > i)
            res[k++] = (void*)arr1[i];
        if (nelts2 > j)
            res[k++] = (void*)arr2[j];
    }
    return res;
}

void add_last_m(const void *data, int nelts, void ***arr) {
	void **new_arr = NULL;
	
    if (NULL == (new_arr = realloc(*arr, (nelts + 1) * sizeof(void*)))) {
        perror("realloc error");
        return;
	}
    new_arr[nelts] = (void*)data;
    *arr = new_arr;
}
