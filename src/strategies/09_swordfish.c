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

static void _cluster_cb(poss_i_t n, pos_t positions[n], void *state_ptr) {
  _state *state = state_ptr;

  cluster_t **cluster_rm = malloc(n*sizeof(cluster_t *));
  memset(cluster_rm, 0, n*sizeof(cluster_t *));

  #define do_return() do { \
    for (int i = 0; i < n; i++) \
      if (cluster_rm[i]) \
        free(cluster_rm[i]); \
    return; } while (0)

  // find exactly n cluster_rm
  for (int i = 0; i < n; i++) {
    for_pos_cluster(cluster(positions[i], state->gen_pre), pos, ({
      if (state->sudoku->arr[pos.i][pos.j] == state->val) do_return();
      if (is_val_possible(state->sudoku->possibilities[pos.i][pos.j], state->val)) {
        cluster_t cluster_rm_init = cluster(pos, state->gen_rm);

        int j;
        for (j = 0; j < n; j++) {
          if (!cluster_rm[j]) {
            // Terrible hack :(
            cluster_rm[j] = malloc(sizeof(cluster_t));
            memcpy(cluster_rm[j], &cluster_rm_init, sizeof(cluster_t));
            break;
          } else if (is_cluster_equal(cluster_rm_init, *cluster_rm[j])) {
            break;
          }
        }
        // more than n clusters
        if (j == n) {
          do_return();
        }
      }
    }))
  }

  // less than n clusters. How is this possible?
  assert(cluster_rm[n-1]);

  bool f = false;
  // positions in the cluster_rm's not part of the cluster_pre's cannot have val
  for (int i = 0; i < n; i++) {
    for_pos_cluster(*cluster_rm[i], pos, ({
      int j;
      for (j = 0; j < n; j++) {
        if (is_pos_cluster(pos, cluster(positions[j], state->gen_pre)))
          break;
      }
      if (j == n)
        f |= decr_possible(state->sudoku, pos, state->val);
    }))
  }
  if (f) debug_print(
    printf_val " %s",
    state->val, state->gen_pre.name
  ); // TODO: More debugging information

  do_return();
}

static void _cluster_gen(sudoku_t *sudoku, val_t val,
  cluster_gen_t gen_pre, cluster_gen_t gen_rm
) {
  _state state = { .sudoku = sudoku,
    .gen_pre = gen_pre, .gen_rm = gen_rm, .val = val };

  combination_cluster(3, *gen_pre.complement, &_cluster_cb, &state);
}

STRATEGY("Swordfish", 9) {
  for_val(val) {
    _cluster_gen(sudoku, val, vert_c, horz_c);
    _cluster_gen(sudoku, val, horz_c, vert_c);
  }

  return stack_size(sudoku->decr_poss);
}
