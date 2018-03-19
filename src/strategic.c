#include "strategic.h"
#include "strategies/all_strategy.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>

bool decr_possible(sudoku *sudoku, pos_type position, val_type val) {
  pos_single_type i = position.i, j = position.j;
  // sanity checks: bounds
  assert(i >= 0 && i < 9);
  assert(j >= 0 && j < 9);
  assert(val > 0 && val <= 9);

  val_type *poss = sudoku->possibilities[i][j];

  // k = # of possibility left
  int k, p = -1;
  val_type v;
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
      sudoku->poststack[sudoku->poststacksize++] = position;
    }

    debug_print("%d %d %d\n", i, j, val);
  }
  return p >= 0;
}

void place(sudoku *sudoku, pos_type position, val_type val) {
  pos_single_type i = position.i, j = position.j;
  if (sudoku->arr[i][j] == val || !val) return;
  assert(!sudoku->arr[i][j]);
  sudoku->arr[i][j] = val;
  sudoku->possibilities[i][j][0] = 0;

  for_pos_cluster(c, cluster(position, vert_c), pos, ({
    decr_possible(sudoku, pos, val);
  }))
  for_pos_cluster(c, cluster(position, horz_c), pos, ({
    decr_possible(sudoku, pos, val);
  }))
  for_pos_cluster(c, cluster(position, cell_c), pos, ({
    decr_possible(sudoku, pos, val);
  }))
}

void solve_sudoku(sudoku_arr sudoku_arr) {
  sudoku *sudoku = (struct sudoku *)malloc(sizeof(struct sudoku));
  sudoku->poststacksize = 0;
  for_pos_cluster_zero(c, all_c, pos, ({
    sudoku->arr[pos.i][pos.j] = 0;
    if (sudoku_arr[pos.i][pos.j]) {
      sudoku->possibilities[pos.i][pos.j] = (val_type *)malloc(2*sizeof(val_type));
      // only possibility
      sudoku->possibilities[pos.i][pos.j][0] = sudoku_arr[pos.i][pos.j];
      sudoku->possibilities[pos.i][pos.j][1] = 0;
      sudoku->poststack[sudoku->poststacksize++] = pos;
    } else {
      // all 9 possibilities
      sudoku->possibilities[pos.i][pos.j] = (val_type *)malloc(10*sizeof(val_type));
      for_val(k) {
        sudoku->possibilities[pos.i][pos.j][k-1] = k;
      }
      sudoku->possibilities[pos.i][pos.j][9] = 0;
    }
  }))
  while (true) {
    bool c = false;

#define _strategy_wrapper(f) c |= f; if (c) continue;
    _strategy_wrapper(naked_single(sudoku))
    _strategy_wrapper(hidden_single(sudoku))
    // _strategy_wrapper(naked_pair_plus(sudoku))
    // _strategy_wrapper(pointing_pair_plus(sudoku))
    // _strategy_wrapper(claiming_pair_plus(sudoku))
#undef _strategy_wrapper

    break;
  }

  for_pos_cluster_zero(c, all_c, pos, ({
    sudoku_arr[pos.i][pos.j] = sudoku->arr[pos.i][pos.j];
  }))
  free(sudoku);
}
