#ifndef SEQUENCEOPS_H
#define SEQUENCEOPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

void swap_items(int a, int b, void **arr);

int* tabulate_lp(int (*func)(int idx), const int cnt);

int index_of_lp(const void *data, int nelts, const void **arr,
    int (*cmp)(const void *a, const void *b));
void* find_lp(const void *data, int nelts, const void **arr,
    int (*cmp)(const void *a, const void *b));

void minmax_lp(const void* minMaxArr[2], int nelts, const void **arr,
    int (*cmp)(const void *a, const void *b));

void reverse_lp(int nelts, void **arr);
void** copy_of(int nelts, const void **arr);

void foreach_lp(void *acc, void (*func)(void *el, void *acc), int nelts,
    const void **arr);

bool is_ordered_lp(int nelts, const void **arr, 
    int (*cmp)(const void *a, const void *b));
void quick_sort(void** arr, int lo, int hi, 
	int (*cmp)(const void* a, const void* b));

void append_lp(int nelts1, void ***arr1, int nelts2, const void **arr2);
void** interleave_lp(const int nelts1, const void **arr1, const int nelts2,
    const void **arr2);

void add_last_m(const void *data, int nelts, void ***arr);

#ifdef __cplusplus
}
#endif

#endif // SEQUENCEOPS_H
