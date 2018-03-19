#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"
#include "../combination.h"

typedef struct _state {
  struct sudoku *sudoku;
  struct cluster_t *cluster;
  bool found;
} _state;

static void _cluster_cb(poss_i_t n, pos_t *positions, void *state_ptr) {
  _state *state = (_state *)state_ptr;
  val_t poss_arr[10] = {0};

  // find all posssibilities in the combinations
  for (poss_i_t i = 0; i < n; i++) {
    pos_t pos = positions[i];
    if (state->sudoku->arr[pos.i][pos.j]) return;
    copy_poss(state->sudoku->possibilities[pos.i][pos.j], poss_arr);
  }

  // pass the test if # of possibilities == number of positions
  if (poss_size(poss_arr) != n) return;

  bool f = false;

  // positions in the cluster not part of the combination cannot have any
  // possibility the combination claimed
  for_pos_cluster(c, *state->cluster, pos, ({
    poss_i_t i;
    for (i = 0; i < n; i++) {
      if (is_pos_equal(positions[i], pos)) break;
    }
    if (!is_pos_equal(positions[i], pos)) {
      f |= truncate_possible(state->sudoku, pos, poss_arr, false);
    }
  }))
  if (f) debug_print("%s " printf_poss_i " (" printf_pos_s ", " printf_pos_s ")\n",
    state->cluster->gen->name, n,
    state->cluster->rel.i, state->cluster->rel.j
  );

  state->found |= f;
}

static bool _cluster_gen(sudoku *sudoku, cluster_gen_t gen) {
  _state state = { .sudoku = sudoku, .found = false };
  for_pos_cluster(initc, *gen.complement, initpos, ({
    cluster_t c = cluster(initpos, gen);
    state.cluster = &c;
    for (poss_i_t n = 2; n <= 4; n++) {
      // select all combinations of 2-4 in the cluster
      combination_cluster(n, c, &_cluster_cb, &state);
    }
  }))
  return state.found;
}

bool naked_pair_plus(sudoku *sudoku) {
  bool f = false;

  f |= _cluster_gen(sudoku, horz_c);
  f |= _cluster_gen(sudoku, vert_c);
  f |= _cluster_gen(sudoku, cell_c);

  return f;
}
