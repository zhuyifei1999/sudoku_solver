#ifndef STRATEGIC_H_
#define STRATEGIC_H_

#include "sudoku.h"

typedef struct sudoku {
  val_type arr[9][9];
  val_type *possibilities[9][9];
  pos_type poststack[9*9*2];
  int poststacksize;
} sudoku;

// remove the val possibility of sudoku at i, j
bool decr_possible(sudoku *sudoku, pos_type i, pos_type j, val_type val);
// determine the value of sudoku at i, j to be val
void place(sudoku *sudoku, pos_type i, pos_type j, val_type val);

void solve_sudoku(val_type sudoku[9][9]);

#endif
