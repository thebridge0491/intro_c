#ifndef SEQUENCEOPS_H
#define SEQUENCEOPS_H

#ifdef __cplusplus
extern "C" {
#endif

void swap_items(int a, int b, void **arr);
void** copy_of(int nelts, const void **arr);

int index_of_lp(const void *data, int nelts, const void **arr,
    int (*cmp)(const void *a, const void *b));

void reverse_lp(int nelts, void **arr);

#ifdef __cplusplus
}
#endif

#endif // SEQUENCEOPS_H
