#include <stdlib.h>
#include <string.h>
#include <log4c.h>

#include "intro_c/sequenceops.h"

static log4c_category_t *prac1;

void swap_items(int a, int b, void **arr) {
    void *swap = arr[a]; arr[a] = arr[b]; arr[b] = swap;
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

int index_of_lp(const void *data, int nelts, const void **arr,
        int (*cmp)(const void *a, const void *b)) {
    for (int i = 0; nelts > i; ++i)
        if (0 == cmp(arr[i], data))
            return i;
    return -1;
}

void reverse_lp(int nelts, void **arr) {
    prac1 = log4c_category_get("prac");
    log4c_category_log(prac1, LOG4C_PRIORITY_INFO, "reverse_lp");
    for (int i = 0, j = nelts - 1; j > i; ++i, --j)
        swap_items(i, j, arr);
}
