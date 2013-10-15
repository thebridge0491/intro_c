//#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <log4c.h>

struct hanoi_args {int src; int dest; int spare; int ndisks; int acc; 
    int splits;
};

static void hanoi_helper(struct hanoi_args args_in, const int exp2_ndisks,
        int **arr) {
    int idx = -1 + args_in.acc;
    
    if (0 == args_in.ndisks)
        return;
    struct hanoi_args ha1 = {.src = args_in.src, .dest = args_in.spare, 
        .spare = args_in.dest, .ndisks = args_in.ndisks - 1, 
        .acc = args_in.acc - exp2_ndisks / powf(2.0f, args_in.splits + 1),
        .splits = args_in.splits + 1};
    struct hanoi_args ha2 = {.src = args_in.spare, .dest = args_in.dest, 
        .spare = args_in.src, .ndisks = args_in.ndisks - 1, 
        .acc = args_in.acc + exp2_ndisks / powf(2.0f, args_in.splits + 1),
        .splits = args_in.splits + 1};
    arr[idx][0] = args_in.src;
    arr[idx][1] = args_in.dest;
    hanoi_helper(ha1, exp2_ndisks, arr);
    hanoi_helper(ha2, exp2_ndisks, arr);
}

int** hanoi(const int src, const int dest, const int spare, const int ndisks) {
    const int exp2_ndisks = powf(2.0f, ndisks);
    const int len_arr = exp2_ndisks - 1, splits = 1;
    int **arr = NULL;
    
    if (NULL == (arr = malloc(len_arr * sizeof(int*)))) {
        perror("malloc error(arr2d)");
        return NULL;
	}
    //if (NULL == (arr[0] = malloc(len_arr * 2 * sizeof(int)))) {
    //	perror("malloc error(arr2d[0])");
	//	return NULL;
	//}
    for (int i = 0; len_arr > i; ++i)
        if (NULL == (arr[i] = malloc(2 * sizeof(int)))) {
            perror("malloc error(arr2d[i])");
            return NULL;
		}
    
    struct hanoi_args args_in = {.src = src, .dest = dest, .spare = spare,
        .ndisks = ndisks, .acc = exp2_ndisks / powf(2.0f, splits),
        .splits = splits};
    hanoi_helper(args_in, exp2_ndisks, arr);
    return arr;
}


static bool safep(const int r, const int c, const int col, const int *board) {
    return (board[c] == r) || (abs(board[c] - r) == col - c);
}
static void nqueens_helper(int *queens_ndx, const int numqueens, 
        const int col, int *board) {
    if (numqueens == col) {
        --(*queens_ndx);
        return;
    }
    for (int r = 0, c = 0; numqueens > r; ++r) {
        for (c = 0; col > c && !safep(r, c, col, board); ++c);
        
        if (col > c)
            continue;
        board[col] = r;
        nqueens_helper(queens_ndx, numqueens, col + 1, board);
        
        if (0 == *queens_ndx)
            return;
    }
}

int* nqueens(const int ndx, const int numqueens) {
    int *board = NULL, queens_ndx = ndx;
    
    if (NULL == (board = calloc(numqueens, sizeof(int)))) {
        perror("calloc error");
        return NULL;
	}
    nqueens_helper(&queens_ndx, numqueens, 0, board);
    return board;
}
