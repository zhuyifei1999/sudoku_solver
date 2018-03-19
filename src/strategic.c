#include "strategic.h"
#include "possibility.h"
#include "strategies/all_strategy.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>

bool decr_possible(sudoku_t *sudoku, pos_t position, val_t val) {
  pos_s_t i = position.i, j = position.j;
  // sanity checks: bounds
  assert(i >= 0 && i < 9);
  assert(j >= 0 && j < 9);
  assert(val > 0 && val <= 9);

  val_t *poss = sudoku->possibilities[i][j];

  poss_i_t k; // # of possibility left
  signed_poss_i_t p = -1; // position of concerned possibility
  val_t v;
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

    debug_print("(" printf_pos_s ", " printf_pos_s ") " printf_val "\n", i, j, val);
  }
  return p >= 0;
}

bool truncate_possible(sudoku_t *sudoku, pos_t position, val_t *poss_arr, bool intersect) {
  bool f = false;
  if (intersect) {
    for_val(val) {
      if (is_val_possible(poss_arr, val)) continue;
      f |= decr_possible(sudoku, position, val);
    }
  } else {
    val_t post, val;
    for (post = 0; (val = poss_arr[post]); post++) {
      f |= decr_possible(sudoku, position, val);
    }
  }
  return f;
}

void place(sudoku_t *sudoku, pos_t position, val_t val) {
  pos_s_t i = position.i, j = position.j;
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
  sudoku_t *sudoku = (sudoku_t *)malloc(sizeof(sudoku_t));
  sudoku->poststacksize = 0;
  for_pos_cluster_zero(c, all_c, pos, ({
    sudoku->arr[pos.i][pos.j] = 0;
    if (sudoku_arr[pos.i][pos.j]) {
      sudoku->possibilities[pos.i][pos.j] = (val_t *)malloc(2*sizeof(val_t));
      // only possibility
      sudoku->possibilities[pos.i][pos.j][0] = sudoku_arr[pos.i][pos.j];
      sudoku->possibilities[pos.i][pos.j][1] = 0;
      sudoku->poststack[sudoku->poststacksize++] = pos;
    } else {
      // all 9 possibilities
      sudoku->possibilities[pos.i][pos.j] = (val_t *)malloc(10*sizeof(val_t));
      for_val(k) {
        sudoku->possibilities[pos.i][pos.j][k-1] = k;
      }
      sudoku->possibilities[pos.i][pos.j][9] = 0;
    }
  }))

  while (true) {
    bool (*strategy)(sudoku_t *);
    for (size_t i = 0; (strategy = strategies[i]); i++) {
      if ((*strategy)(sudoku)) break;
    }
    if (!strategy) break;
  }

  for_pos_cluster_zero(c, all_c, pos, ({
    sudoku_arr[pos.i][pos.j] = sudoku->arr[pos.i][pos.j];
    free(sudoku->possibilities[pos.i][pos.j]);
  }))
  free(sudoku);
}
