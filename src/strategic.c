#include "strategic.h"
#include "possibility.h"
#include "strategies/all_strategy.h"
#include "debug.h"

#include <assert.h>
#include <stdlib.h>

static void _decr_possible_commit(sudoku_t *sudoku) {
  while (stack_size(sudoku->decr_poss)) {
    pos_val_t item = stack_pop(sudoku->decr_poss);
    pos_s_t i = item.pos.i, j = item.pos.j;
    val_t val = item.val;

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
        stack_push(sudoku->ns_pos, item.pos);
      }
    }
  }
}

bool decr_possible(sudoku_t *sudoku, pos_t position, val_t val) {
  pos_s_t i = position.i, j = position.j;
  // sanity checks: bounds
  assert(i >= 0 && i < 9);
  assert(j >= 0 && j < 9);
  assert(val > 0 && val <= 9);

  val_t *poss = sudoku->possibilities[i][j];

  if (sudoku->arr[i][j]) return false;
  if (!is_val_possible(poss, val)) return false;

  // sanity check: it has at least one possibility left after removal
  assert(poss_size(poss) > 1);

  stack_push(sudoku->decr_poss, ((pos_val_t){ .pos = position, .val = val }));
  debug_print("(" printf_pos_s ", " printf_pos_s ") " printf_val "\n", i, j, val);
  return true;
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
  sudoku->possibilities[i][j][0] = val;
  sudoku->possibilities[i][j][1] = 0;

  for_pos_cluster(cluster(position, vert_c), pos, ({
    decr_possible(sudoku, pos, val);
  }))
  for_pos_cluster(cluster(position, horz_c), pos, ({
    decr_possible(sudoku, pos, val);
  }))
  for_pos_cluster(cluster(position, cell_c), pos, ({
    decr_possible(sudoku, pos, val);
  }))
}

void solve_sudoku(sudoku_arr sudoku_arr) {
  sudoku_t *sudoku = malloc(sizeof(sudoku_t));
  stack_init(sudoku->ns_pos);
  stack_init(sudoku->decr_poss);
  for_pos_cluster_zero(all_c, pos, ({
    sudoku->arr[pos.i][pos.j] = 0;
    if (sudoku_arr[pos.i][pos.j]) {
      sudoku->possibilities[pos.i][pos.j] = malloc(2*sizeof(val_t));
      // only possibility
      sudoku->possibilities[pos.i][pos.j][0] = sudoku_arr[pos.i][pos.j];
      sudoku->possibilities[pos.i][pos.j][1] = 0;
      stack_push(sudoku->ns_pos, pos);
    } else {
      // all 9 possibilities
      sudoku->possibilities[pos.i][pos.j] = malloc(10*sizeof(val_t));
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
    if (stack_size(sudoku->decr_poss)) _decr_possible_commit(sudoku);
  }

  for_pos_cluster_zero(all_c, pos, ({
    sudoku_arr[pos.i][pos.j] = sudoku->arr[pos.i][pos.j];
    free(sudoku->possibilities[pos.i][pos.j]);
  }))
  free(sudoku);
}
