#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"
#include "../combination.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct _state {
  sudoku_t *sudoku;
  cluster_gen_t gen_pre;
  cluster_gen_t gen_rm;
  val_t val;
} _state;

static bool _is_pair(_state *state, cluster_t cluster_pre,
  cluster_t **cluster_rm_1, cluster_t **cluster_rm_2
) {
  for_pos_cluster(cluster_pre, pos, ({
    if (state->sudoku->arr[pos.i][pos.j] == state->val) return false;

    if (is_val_possible(state->sudoku->possibilities[pos.i][pos.j], state->val)) {
      cluster_t **cluster_rm_ptr;
      if (!*cluster_rm_1) {
        cluster_rm_ptr = cluster_rm_1;
      } else if (!*cluster_rm_2) {
        // if (!is_pos_cluster(pos, **cluster_rm_1)) return false;
        cluster_rm_ptr = cluster_rm_2;
      } else {
        // more than 2
        return false;
      }
      // Terrible hack :(
      cluster_t cluster_rm_init = cluster(pos, state->gen_rm);
      *cluster_rm_ptr = malloc(sizeof(cluster_t));
      memcpy(*cluster_rm_ptr, &cluster_rm_init, sizeof(cluster_t));
    }
  }))
  // it would be a naked single otherwise
  assert(!*cluster_rm_1 || *cluster_rm_2);
  // exactly two
  return *cluster_rm_1 && *cluster_rm_2;
}

static void _cluster_cb(poss_i_t n, pos_t *positions, void *state_ptr) {
  _state *state = state_ptr;

  cluster_t *cluster_rm_a_1 = NULL;
  cluster_t *cluster_rm_a_2 = NULL;
  cluster_t *cluster_rm_b_1 = NULL;
  cluster_t *cluster_rm_b_2 = NULL;

  #define do_return() do { \
    if (cluster_rm_a_1) free(cluster_rm_a_1); \
    if (cluster_rm_a_2) free(cluster_rm_a_2); \
    if (cluster_rm_b_1) free(cluster_rm_b_1); \
    if (cluster_rm_b_2) free(cluster_rm_b_2); \
    return; } while (0)

  cluster_t cluster_pre_a = cluster(positions[0], state->gen_pre);
  cluster_t cluster_pre_b = cluster(positions[1], state->gen_pre);

  // these clusters should be pairs (only two position have the possibility)
  if (!(_is_pair(state, cluster_pre_a, &cluster_rm_a_1, &cluster_rm_a_2) &&
        _is_pair(state, cluster_pre_b, &cluster_rm_b_1, &cluster_rm_b_2))) {
    do_return();
  }

  // the corresponding cluster_rm should be the same
  if (!(is_cluster_equal(*cluster_rm_a_1, *cluster_rm_b_1) &&
        is_cluster_equal(*cluster_rm_a_2, *cluster_rm_b_2))) {
    do_return();
  }

  bool f = false;
  // positions in the cluster_rm's not part of the cluster_pre's cannot have val
  for_pos_cluster(*cluster_rm_a_1, pos, ({
    if (is_pos_cluster(pos, cluster_pre_a) || is_pos_cluster(pos, cluster_pre_b))
      continue;
    f |= decr_possible(state->sudoku, pos, state->val);
  }))
  for_pos_cluster(*cluster_rm_a_2, pos, ({
    if (is_pos_cluster(pos, cluster_pre_a) || is_pos_cluster(pos, cluster_pre_b))
      continue;
    f |= decr_possible(state->sudoku, pos, state->val);
  }))
  if (f) debug_print(
    "%s " printf_pos " " printf_pos " | "
    "%s " printf_pos " " printf_pos ": " printf_val,
    state->gen_pre.name, positions[0].i, positions[0].j,
      positions[1].i, positions[1].j,
    state->gen_rm.name, cluster_rm_a_1->rel.i, cluster_rm_a_1->rel.j,
      cluster_rm_a_2->rel.i, cluster_rm_a_2->rel.j,
    state->val
  );

  do_return();
}

static void _cluster_gen(sudoku_t *sudoku, val_t val,
  cluster_gen_t gen_pre, cluster_gen_t gen_rm
) {
  _state state = { .sudoku = sudoku,
    .gen_pre = gen_pre, .gen_rm = gen_rm, .val = val };

  combination_cluster(2, *gen_pre.complement, &_cluster_cb, &state);
}

bool x_wing(sudoku_t *sudoku) {
  for_val(val) {
    // classic x-wing
    _cluster_gen(sudoku, val, vert_c, horz_c);
    _cluster_gen(sudoku, val, horz_c, vert_c);

    /*
    // pointing pairs
    _cluster_gen(sudoku, val, cell_c, horz_c);
    _cluster_gen(sudoku, val, cell_c, vert_c);
    // // claiming pairs
    _cluster_gen(sudoku, val, horz_c, cell_c);
    _cluster_gen(sudoku, val, vert_c, cell_c);
    */
  }

  return stack_size(sudoku->decr_poss);
}
