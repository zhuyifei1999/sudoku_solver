#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"
#include "../combination.h"

#include <assert.h>

typedef struct _state {
  sudoku_t *sudoku;
  cluster_t cluster;
} _state;

static void _poss_cb(poss_i_t n, val_t *poss_arr, void *state_ptr) {
  _state *state = state_ptr;

  unsigned short num_intersect = 0;

  // pass if # of possibility intersections = # of possibilities in poss_arr
  for_pos_cluster(state->cluster, pos, ({
    if (state->sudoku->arr[pos.i][pos.j]) continue;
    if (has_intersection_poss(state->sudoku->possibilities[pos.i][pos.j], poss_arr))
      num_intersect++;
  }))

  if (num_intersect != n) return;

  bool f = false;

  // intersections cannot have any other possibilites other than poss_arr
  for_pos_cluster(state->cluster, pos, ({
    if (state->sudoku->arr[pos.i][pos.j]) continue;
    if (has_intersection_poss(state->sudoku->possibilities[pos.i][pos.j], poss_arr))
      f |= truncate_possible(state->sudoku, pos, poss_arr, true);
  }))

  if (f) {
    debug_print(
      "%s " printf_poss_i " " printf_pos,
      state->cluster.gen->name, n,
      state->cluster.rel.i, state->cluster.rel.j
    );
    debug_print_arr(100, 10, printf_val, poss_arr);
  }
}

static void _cluster(sudoku_t *sudoku, cluster_t cluster) {
  val_t poss_arr[10] = {0};

  // get all possibilities in the cluster
  for_pos_cluster(cluster, pos, ({
    if (sudoku->arr[pos.i][pos.j]) continue;
    copy_poss(sudoku->possibilities[pos.i][pos.j], poss_arr);
  }))

  // debug_print_arr(100, 10, printf_val, poss_arr);

  _state state = { .sudoku = sudoku, .cluster = cluster };
  for (poss_i_t n = 2; n <= 4; n++) {
    // select all combinations of 2-4 in the possibility_arr
    combination_possibility(n, poss_arr, &_poss_cb, &state);
  }
}


bool hidden_candidates(sudoku_t *sudoku) {
  for_pos_cluster(*horz_c.complement, initpos, ({
    _cluster(sudoku, cluster(initpos, horz_c));
  }))
  for_pos_cluster(*vert_c.complement, initpos, ({
    _cluster(sudoku, cluster(initpos, vert_c));
  }))
  for_pos_cluster(*cell_c.complement, initpos, ({
    _cluster(sudoku, cluster(initpos, cell_c));
  }))

  return stack_size(sudoku->decr_poss);
}
