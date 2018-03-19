#ifndef STRATEGIC_H_
#define STRATEGIC_H_

#include "sudoku.h"
#include "possibility.h"

typedef struct sudoku_t {
  val_t arr[9][9];
  val_t *possibilities[9][9];
  pos_t poststack[9*9];
  poss_i_t poststacksize;
} sudoku_t;

// remove the val possibility of sudoku at position
bool decr_possible(sudoku_t *sudoku, pos_t position, val_t val);

// remove the possibilities from position; if intersect is true, keep only intersections
bool truncate_possible(sudoku_t *sudoku, pos_t position, val_t *poss_arr, bool intersect);

// determine the value of sudoku at position to be val
void place(sudoku_t *sudoku, pos_t position, val_t val);

void solve_sudoku(sudoku_arr sudoku);

#endif
