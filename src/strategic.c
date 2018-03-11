#include "strategic.h"
#include "strategies/all_strategy.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>


bool decr_possible(sudoku *sudoku, pos_type i, pos_type j, val_type val) {
  // sanity checks: bounds
  assert(i >= 0 && i < 9);
  assert(j >= 0 && j < 9);
  assert(val > 0 && val <= 9);

  int *poss = sudoku->possibilities[i][j];

  // k = # of possibility left
  int k, v, p = -1;
  for (k = 0; (v = poss[k]); k++) {
    if (v == val) p = k;
  }

  // sanity check: either it is filled or has at least one possibility
  assert(sudoku->arr[i][j] || k > 0);
  if (p >= 0) {
    // get rid of the found item
    poss[p] = poss[k-1];
    poss[k-1] = 0;

    // push to stack for 'naked single' method
    if (k == 2) {
      sudoku->poststack[sudoku->poststacksize++] = i;
      sudoku->poststack[sudoku->poststacksize++] = j;
    }

    debug_print("%d %d %d\n", i, j, val);
  }
  return p >= 0;
}

void place(sudoku *sudoku, pos_type i, pos_type j, val_type val) {
  if (sudoku->arr[i][j] == val || !val) return;
  assert(!sudoku->arr[i][j]);
  sudoku->arr[i][j] = val;
  sudoku->possibilities[i][j][0] = 0;

  _for_place_vert(ai, aj, i, j) {
    decr_possible(sudoku, ai, aj, val);
  }
  _for_place_horz(ai, aj, i, j) {
    decr_possible(sudoku, ai, aj, val);
  }
  _for_place_cell(ai, aj, i, j) {
    decr_possible(sudoku, ai, aj, val);
  }
}

void solve_sudoku(val_type sudoku_arr[9][9]) {
  sudoku *sudoku = (struct sudoku *)malloc(sizeof(struct sudoku));
  sudoku->poststacksize = 0;
  _for_all_places(i, j) {
    sudoku->arr[i][j] = 0;
    if (sudoku_arr[i][j]) {
      sudoku->possibilities[i][j] = (int *)malloc(2*sizeof(int));
      // only possibility
      sudoku->possibilities[i][j][0] = sudoku_arr[i][j];
      sudoku->possibilities[i][j][1] = 0;
      sudoku->poststack[sudoku->poststacksize++] = i;
      sudoku->poststack[sudoku->poststacksize++] = j;
    } else {
      // all 9 possibilities
      sudoku->possibilities[i][j] = (int *)malloc(10*sizeof(int));
      _for_val(k) {
        sudoku->possibilities[i][j][k-1] = k;
      }
      sudoku->possibilities[i][j][9] = 0;
    }
  }
  while (true) {
    bool c = false;

#define _strategy_wrapper(f) c |= f; if (c) continue;

    _strategy_wrapper(naked_single(sudoku))
    _strategy_wrapper(hidden_single(sudoku))

#undef _strategy_wrapper

    break;
  }

  _for_all_places(i, j) {
    sudoku_arr[i][j] = sudoku->arr[i][j];
  }
}
