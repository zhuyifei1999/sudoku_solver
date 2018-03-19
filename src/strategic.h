#ifndef STRATEGIC_H_
#define STRATEGIC_H_

#include "sudoku.h"

typedef struct sudoku {
  val_type arr[9][9];
  val_type *possibilities[9][9];
  pos_type poststack[9*9];
  int poststacksize;
} sudoku;

// remove the val possibility of sudoku at position
bool decr_possible(sudoku *sudoku, pos_type position, val_type val);

// remove the possibilities from position; if intersect is true, keep only intersections
bool truncate_possible(sudoku *sudoku, pos_type position, val_type *poss_arr, bool intersect);

// determine the value of sudoku at position to be val
void place(sudoku *sudoku, pos_type position, val_type val);

void solve_sudoku(sudoku_arr sudoku);

#endif
