#ifndef CLASSIC_PUZZLES_H
#define CLASSIC_PUZZLES_H

#ifdef __cplusplus
extern "C" {
#endif

int** hanoi(const int src, const int dest, const int spare, const int ndisks);

int* nqueens(const int ndx, const int numqueens);

#ifdef __cplusplus
}
#endif

#endif // CLASSIC_PUZZLES_H
