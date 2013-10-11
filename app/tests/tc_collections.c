#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <check.h>
#include <glib.h>
#include <search.h>

#include "intro_c/util.h"

static int initNum1 = 0, initNum2 = 0;
static char *chptr_alloc;
static int arr_ints[] = {2, 1, 0, 4, 3}, arr_9s[] = {9, 9, 9, 9};
static float arr_floats[] = {25.7f, 0.1f, 78.5f, 52.3f};
static char arr_chars[] = {'a', 'e', 'k', 'p', 'u', 'k', 'a'};
static int len_9s = sizeof(arr_9s) / sizeof(arr_9s[0]);
static int len_ints = sizeof(arr_ints) / sizeof(arr_ints[0]);
static int len_floats = sizeof(arr_floats) / sizeof(arr_floats[0]);
static int len_chars = sizeof(arr_chars) / sizeof(arr_chars[0]);

static void setupTCase(void) {
    printf("###setup TCase###\n");
    initNum1 = 5;
    return;
}

static void teardownTCase(void) {
    initNum1 = 0;
    printf("###teardown TCase###\n\n");
    return;
}

static void setupTestA(void) {
    printf("setup Test...\n");
    initNum2 = -1;
    chptr_alloc = calloc(1, sizeof(char));
    return;
}

static void teardownTestA(void) {
    initNum2 = 0;
    free(chptr_alloc);
    printf("...teardown Test\n");
    return;
}

START_TEST (test_garrays) {
    GArray *gint_arr = g_array_new(FALSE, FALSE, sizeof(gint));
    GArray *garr_tmp = NULL;
    
    ck_assert_msg(NULL != (garr_tmp = g_array_new(FALSE, FALSE, sizeof(gint))),
        "make-array");
    g_array_free(garr_tmp, TRUE);
    ck_assert_msg(0 == gint_arr->len, "empty");
    for (int i = 0; len_9s > i; ++i)
        gint_arr = g_array_append_val(gint_arr, arr_9s[i]);
    ck_assert_msg(0 != gint_arr->len, "append");
    ck_assert_msg(len_9s == gint_arr->len, "length");
    ck_assert_msg(9 == gint_arr->data[0], "get");
    gint_arr = g_array_append_vals(gint_arr, arr_ints, len_ints);
    ck_assert_msg((len_9s + len_ints) == gint_arr->len, "concat");
    ((int*)gint_arr->data)[0] = 10;
    ck_assert_msg(10 == ((int*)gint_arr->data)[0], "set");
    int arr_exp[] = {10, 9, 9, 9, 2, 1, 0, 4, 3};
    for (int i = 0; gint_arr->len > i; ++i)
        ck_assert_msg(arr_exp[i] == ((int*)gint_arr->data)[i], 
            "foreach: arr_exp[%i](%i) vice gint_arr->data[%i](%i)", 
            i, arr_exp[i], i, ((int*)gint_arr->data)[i]);
    void *found = lfind(&arr_ints[3], gint_arr->data, (size_t*)&gint_arr->len,
        sizeof(arr_ints[0]), int_cmp);
    if (NULL != found)
		printf("found: %d\n", *(int*)found);
	else
		printf("not found!\n");
    ck_assert_msg(arr_ints[3] == *(int*)found, "find(lfind)");
    char *sort_str = "[0,1,2,3,4,9,9,9,10,]";
    //qsort(gint_arr->data, gint_arr->len, sizeof(int), int_cmp);
    g_array_sort(gint_arr, (GCompareFunc)int_cmp);
    mkstring_ints(gint_arr->len, (const int*)gint_arr->data, &chptr_alloc);
    ck_assert_msg(0 == strcmp(sort_str, chptr_alloc), "sort");
    found = bsearch(&arr_ints[3], gint_arr->data, gint_arr->len, 
        sizeof(arr_ints[0]), int_cmp);
    ck_assert_msg(arr_ints[3] == *(int*)found, "find(bsearch)");
    
    g_array_free(gint_arr, TRUE);
} END_TEST

START_TEST (test_gptrarrays) {
    GPtrArray *gint_ptrarr = g_ptr_array_new_with_free_func(
        (GDestroyNotify)g_free);
    GPtrArray *garr_ptrtmp = NULL;
    
    ck_assert_msg(NULL != (garr_ptrtmp = g_ptr_array_new_with_free_func(
        (GDestroyNotify)g_free)), "make-array");
    g_ptr_array_free(garr_ptrtmp, TRUE);
    ck_assert_msg(0 == gint_ptrarr->len, "empty");
    for (int i = 0; len_9s > i; ++i)
        g_ptr_array_add(gint_ptrarr, &arr_9s[i]);
    ck_assert_msg(0 != gint_ptrarr->len, "append");
    ck_assert_msg(len_9s == gint_ptrarr->len, "length");
    ck_assert_msg(9 == *(int*)g_ptr_array_index(gint_ptrarr, 0), "get");
    for (int i = 0; len_ints > i; ++i)
        g_ptr_array_add(gint_ptrarr, &arr_ints[i]);
    ck_assert_msg((len_9s + len_ints) == gint_ptrarr->len, "concat");
    int el = 10;
    gint_ptrarr->pdata[0] = &el;
    ck_assert_msg(el == *(int*)gint_ptrarr->pdata[0], "set");
    char *foreach_str = "10,9,9,9,2,1,0,4,3,";
    g_ptr_array_foreach(gint_ptrarr, (GFunc)intptr_el_to_str, &chptr_alloc);
    ck_assert_msg(0 == strcmp(foreach_str, chptr_alloc), "foreach");
    free(chptr_alloc);
    chptr_alloc = calloc(1, sizeof(char));
    int *ptr_search = &arr_ints[3];
    int **found = lfind(&ptr_search, gint_ptrarr->pdata, 
        (size_t*)&gint_ptrarr->len, sizeof(void*), intptr_cmp);
    ck_assert_msg(arr_ints[3] == **found, "find(lfind)");
    char *sort_str = "[0,1,2,3,4,9,9,9,10,]";
    //qsort(gint_ptrarr->pdata, gint_ptrarr->len, sizeof(void*), intptr_cmp);
    g_ptr_array_sort(gint_ptrarr, (GCompareFunc)intptr_cmp);
    mkstring_ptrarray(intptr_el_to_str, gint_ptrarr->len, 
        (const void**)gint_ptrarr->pdata, &chptr_alloc);
    ck_assert_msg(0 == strcmp(sort_str, chptr_alloc), "sort");
    found = bsearch(&ptr_search, gint_ptrarr->pdata, gint_ptrarr->len, 
        sizeof(void*), intptr_cmp);
    ck_assert_msg(arr_ints[3] == **found, "find(bsearch)");
    
    g_ptr_array_free(gint_ptrarr, FALSE);
} END_TEST

static void sum_el_acc(void *el, void *acc) {
    *(int*)acc = *(int*)el + *(int*)acc;
}

START_TEST (test_glists) {
    GList *gint_lst = NULL;
    GList *gint_tmp = NULL;
    
    //ck_assert_msg(false, "make-list");
    ck_assert_msg(NULL == gint_lst, "empty");
    for (int i = 0; len_9s > i; ++i)
        gint_lst = g_list_append(gint_lst, &arr_9s[i]);
    for (int i = 0; len_ints > i; ++i)
        gint_tmp = g_list_append(gint_tmp, &arr_ints[i]);
    ck_assert_msg(0 != g_list_length(gint_lst), "append");
    ck_assert_msg(len_9s == g_list_length(gint_lst), "length");
    ck_assert_msg(arr_9s[0] == *(int*)g_list_first(gint_lst)->data,"head");
    ck_assert_msg(arr_9s[2] == *(int*)g_list_nth_data(gint_lst, 2), "nth");
    gint_lst = g_list_concat(gint_lst, gint_tmp);
    ck_assert_msg((len_9s + len_ints) == g_list_length(gint_lst), "concat");
    char *exp_str = "3,4,0,1,2,9,9,9,9,";
    gint_lst = g_list_reverse(gint_lst);
    g_list_foreach(gint_lst, (GFunc)intptr_el_to_str, &chptr_alloc);
    ck_assert_msg(0 == strcmp(exp_str, chptr_alloc), "reverse");
    free(chptr_alloc);
    chptr_alloc = calloc(1, sizeof(char));
    ck_assert_msg(1 == g_list_index(gint_lst, &arr_ints[3]), "index");
    ck_assert_msg(4 == *(int*)g_list_find(gint_lst, &arr_ints[3])->data, 
        "find");
    int res_sum = 0;
    //for (GList *itr = gint_lst; itr; itr = itr->next)
    //    sum_el_acc((gint*)itr->data, &res_sum);
    g_list_foreach(gint_lst, (GFunc)sum_el_acc, &res_sum);
    ck_assert_msg(46 == res_sum, "foreach");
    exp_str = "0,1,2,3,4,9,9,9,9,";
    gint_lst = g_list_sort(gint_lst, (GCompareFunc)int_cmp);
    g_list_foreach(gint_lst, (GFunc)intptr_el_to_str, &chptr_alloc);
    ck_assert_msg(0 == strcmp(exp_str, chptr_alloc), "sort(%s)", chptr_alloc);
    
    //g_list_free_full(gint_lst, (GDestroyNotify)g_free);
    g_list_free(gint_tmp);
    g_list_free(gint_lst);
} END_TEST

START_TEST (test_gqueues) {
    GQueue *gfloat_queue = g_queue_new();
    GQueue *gfloat_tmp = NULL;
    
    ck_assert_msg(NULL != (gfloat_tmp = g_queue_new()), "make-queue");
    g_queue_free_full(gfloat_tmp, (GDestroyNotify)g_free);
    ck_assert_msg(g_queue_is_empty(gfloat_queue), "empty");
    for (int i = 0; len_floats > i; ++i)
        g_queue_push_tail(gfloat_queue, &arr_floats[i]);
    ck_assert_msg(arr_floats[0] == *(float*)g_queue_peek_head(gfloat_queue), 
        "peek");
    float el = -5.0f;
    g_queue_push_tail(gfloat_queue, &el);
    ck_assert_msg(el == *(float*)g_queue_peek_tail(gfloat_queue), "push");
    ck_assert_msg(0 == float_cmp(0.0001f * arr_floats[0], &arr_floats[0], 
        g_queue_pop_head(gfloat_queue)), "pop");
    char *exp_str = "0.10,78.50,52.30,-5.00,";
    g_queue_foreach(gfloat_queue, (GFunc)floatptr_el_to_str, &chptr_alloc);
    ck_assert_msg(0 == strcmp(exp_str, chptr_alloc), "foreach");
    
    //g_queue_free_full(gfloat_queue, (GDestroyNotify)g_free);
    g_queue_free(gfloat_queue);
} END_TEST

START_TEST (test_ghashtbls) {
    GHashTable *gstrchar_htbl = g_hash_table_new_full((GHashFunc)g_str_hash, 
        (GEqualFunc)g_str_equal, (GDestroyNotify)g_free, NULL);
    GHashTable *gstrchar_tmp = NULL;
    char *val_char = NULL, key_str[32] = {'\0'}, *key_new;
    
    ck_assert_msg(NULL != (gstrchar_tmp = g_hash_table_new_full(
        (GHashFunc)g_str_hash, (GEqualFunc)g_str_equal, (GDestroyNotify)g_free, 
        (GDestroyNotify)g_free)), "make-hashtbl");
    g_hash_table_destroy(gstrchar_tmp);
    for (int i = 0; len_chars > i; ++i) {
        val_char = &arr_chars[i];
        snprintf(key_str, 31, "ltr %i", i % 5);
        key_new = calloc(1 + strlen(key_str), sizeof(char));
        strncpy(key_new, key_str, 1 + strlen(key_str));
        g_hash_table_replace(gstrchar_htbl, key_new, val_char);
    }
    ck_assert_msg(5 == g_hash_table_size(gstrchar_htbl), "size");
    *val_char = 'Z';
    snprintf(key_str, 31, "ltr 02");
    key_new = calloc(1 + strlen(key_str), sizeof(char));
    strncpy(key_new, key_str, 1 + strlen(key_str));
    ck_assert_msg(g_hash_table_replace(gstrchar_htbl, key_new, val_char), 
        "replace");
    ck_assert_msg(NULL != g_hash_table_lookup(gstrchar_htbl, "ltr 2"), "find");
    ck_assert_msg(g_hash_table_contains(gstrchar_htbl,"ltr 2"),"contains");
    ck_assert_msg(g_hash_table_remove(gstrchar_htbl, "ltr 2"), "remove");
    char *exp_str = "(ltr 02 => Z),(ltr 0 => k),(ltr 1 => Z),"
        "(ltr 3 => p),(ltr 4 => u),";
    /*
    GHashTableIter itr_htbl;
    void *key, *val;
    g_hash_table_iter_init(&itr_htbl, gstrchar_htbl);
    while (g_hash_table_iter_next(&itr_htbl, &key, &val))
        strcharptr_kv_to_str_void(key, val, &chptr_alloc);
    */
    g_hash_table_foreach(gstrchar_htbl, (GHFunc)strcharptr_kv_to_str_void, 
        &chptr_alloc);
    ck_assert_msg(0 == strcmp(exp_str, chptr_alloc), "foreach");
    
    g_hash_table_destroy(gstrchar_htbl);
} END_TEST

START_TEST (test_gtrees) {
    GTree *gstrchar_tree = g_tree_new_full((GCompareDataFunc)g_ascii_strcasecmp,
            NULL, (GDestroyNotify)g_free, NULL);
    GTree *gstrchar_tmp = NULL;
    char *val_char = NULL, key_str[32] = {'\0'}, *key_new;
    
    ck_assert_msg(NULL != (gstrchar_tmp = g_tree_new_full(
        (GCompareDataFunc)g_ascii_strcasecmp, NULL, (GDestroyNotify)g_free, 
        (GDestroyNotify)g_free), "make-tree"));
    g_tree_destroy(gstrchar_tmp);
    ck_assert_msg(0 == g_tree_height(gstrchar_tree), "height");
    for (int i = 0; len_chars > i; ++i) {
        val_char = &arr_chars[i];
        snprintf(key_str, 31, "ltr %i", i % 5);
        key_new = calloc(1 + strlen(key_str), sizeof(char));
        strncpy(key_new, key_str, 1 + strlen(key_str));
        g_tree_replace(gstrchar_tree, key_new, val_char);
    }
    ck_assert_msg(5 == g_tree_nnodes(gstrchar_tree), "num nodes(%i)", 
        g_tree_nnodes(gstrchar_tree));
    *val_char = 'Z';
    snprintf(key_str, 31, "ltr 02");
    key_new = calloc(1 + strlen(key_str), sizeof(char));
    strncpy(key_new, key_str, 1 + strlen(key_str));
    g_tree_replace(gstrchar_tree, key_new, val_char);
    ck_assert_msg(NULL != g_tree_lookup(gstrchar_tree, "ltr 02"), "replace");
    ck_assert_msg('Z' == *(char*)g_tree_lookup(gstrchar_tree, "ltr 02"), 
        "find");
    ck_assert_msg(g_tree_remove(gstrchar_tree, "ltr 2"), "remove");
    char *exp_str = "(ltr 0 => k),(ltr 02 => Z),(ltr 1 => Z),"
        "(ltr 3 => p),(ltr 4 => u),";
    g_tree_foreach(gstrchar_tree, (GTraverseFunc)strcharptr_kv_to_str_bool, 
        &chptr_alloc);
    ck_assert_msg(0 == strcmp(exp_str, chptr_alloc), "foreach(%s -- %s)", exp_str, chptr_alloc);
    
    g_tree_destroy(gstrchar_tree);
} END_TEST


Suite* tc_collections_suite(void) {
    Suite *suite = suite_create("tc_collections");
    TCase *tc_collections = tcase_create("Tc_collections");
    tcase_set_tags(tc_collections, "Tag1");
    
    tcase_add_unchecked_fixture(tc_collections, setupTCase, teardownTCase);
    tcase_add_checked_fixture(tc_collections, setupTestA, teardownTestA);
    
    TFun tests[] = {
		test_garrays, test_gptrarrays, test_glists, test_gqueues, 
		test_ghashtbls, test_gtrees
	};
    int arrLen = sizeof(tests) / sizeof(tests[0]);
    
    /* Add the tests to the tcase */
    for (int i = 0; arrLen > i; ++i)
        tcase_add_test(tc_collections, tests[i]);
    //tcase_add_test(tc_collections, test_method);
    
    /* Add tcase to suite */
    suite_add_tcase(suite, tc_collections);
    return suite;
}
