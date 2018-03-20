#ifndef STRATEGIC_H_
#define STRATEGIC_H_

#include "sudoku.h"
#include "stack.h"

typedef struct pos_val_t {
  pos_t pos;
  val_t val;
} pos_val_t;

typedef struct sudoku_t {
  val_t arr[9][9];
  val_t *possibilities[9][9];
  struct_stack(pos_t, ns_pos, 9*9)
  struct_stack(pos_val_t, decr_poss, 9*9*9)
} sudoku_t;

// remove the val possibility of sudoku at position
bool decr_possible(sudoku_t *sudoku, pos_t position, val_t val);

// remove the possibilities from position; if intersect is true, keep only intersections
bool truncate_possible(sudoku_t *sudoku, pos_t position, val_t *poss_arr, bool intersect);

// determine the value of sudoku at position to be val
void place(sudoku_t *sudoku, pos_t position, val_t val);

void solve_sudoku(sudoku_arr sudoku);

#endif
