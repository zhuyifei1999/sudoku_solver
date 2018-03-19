#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"
#include "../combination.h"

typedef struct _state {
  struct sudoku *sudoku;
  struct cluster *cluster;
  bool found;
} _state;

static void _cluster_cb(int n, pos_type *positions, void *state_ptr) {
  _state *state = (_state *)state_ptr;
  val_type poss_arr[10] = {0};

  // find all posssibilities in the combinations
  for (int i = 0; i < n; i++) {
    pos_type pos = positions[i];
    if (state->sudoku->arr[pos.i][pos.j]) return;
    copy_poss(state->sudoku->possibilities[pos.i][pos.j], poss_arr);
  }

  // pass the test if # of possibilities == number of positions
  if (poss_size(poss_arr) != n) return;

  bool f = false;

  // positions in the culster not part of the combination cannot have any
  // possibility the combination claimed
  for_pos_cluster(c, *state->cluster, pos, ({
    int i;
    for (i = 0; i < n; i++) {
      if (is_pos_equal(positions[i], pos)) break;
    }
    if (!is_pos_equal(positions[i], pos)) {
      f |= truncate_possible(state->sudoku, pos, poss_arr, false);
    }
  }))

  if (f) debug_print("%s %d %d\n", state->cluster->gen->name,
    state->cluster->rel.i, state->cluster->rel.j);

  state->found |= f;
}

static bool _cluster_gen(sudoku *sudoku, cluster_gen gen) {
  _state state = { .sudoku = sudoku, .found = false };
  for_pos_cluster(initc, *gen.complement, initpos, ({
    cluster c = cluster(initpos, gen);
    state.cluster = &c;
    for (int n = 2; n <= 4; n++) {
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
