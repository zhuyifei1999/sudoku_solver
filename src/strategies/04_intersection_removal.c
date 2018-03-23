#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void _cluster(sudoku_t *sudoku, val_t val,
  cluster_t cluster_pre, cluster_gen_t gen_rm
) {
  cluster_t *cluster_rm = NULL;
  unsigned short num_found = 0;

#define do_return() do { if (cluster_rm) free(cluster_rm); return; } while (0)

  // The value should only belong to one cluster of gen_rm
  for_pos_cluster(cluster_pre, pos, ({
    if (sudoku->arr[pos.i][pos.j] == val) do_return();

    num_found++;

    if (is_val_possible(sudoku->possibilities[pos.i][pos.j], val)) {
      if (cluster_rm) {
        if (!is_pos_cluster(pos, *cluster_rm)) do_return();
      } else {
        // Terrible hack :(
        cluster_t cluster_rm_init = cluster(pos, gen_rm);
        cluster_rm = malloc(sizeof(cluster_t));
        memcpy(cluster_rm, &cluster_rm_init, sizeof(cluster_t));
      }
    }
  }))

  if (!cluster_rm) do_return();

  // num_found = 1 is a naked single
  assert(num_found > 1);

  bool f = false;
  // any other clusters of gen_pre in cluster_rm cannot have the value
  for_pos_cluster(*cluster_rm, pos, ({
    if (is_pos_cluster(pos, cluster_pre)) continue;
    f |= decr_possible(sudoku, pos, val);
  }))

  if (f) {
    debug_print(
      "%s " printf_pos " %s " printf_pos " " printf_val,
      cluster_pre.gen->name, cluster_pre.rel.i, cluster_pre.rel.j,
      cluster_rm->gen->name, cluster_rm->rel.i, cluster_rm->rel.j,
      val
    );
  }

  do_return();
}

static void _cluster_gen(sudoku_t *sudoku, val_t val,
  cluster_gen_t gen_pre, cluster_gen_t gen_rm
) {
  for_pos_cluster(*gen_pre.complement, initpos, ({
    _cluster(sudoku, val, cluster(initpos, gen_pre), gen_rm);
  }))
}

STRATEGY("Intersection Removal", 4) {
  for_val(val) {
    // pointing pairs
    _cluster_gen(sudoku, val, box_c, row_c);
    _cluster_gen(sudoku, val, box_c, col_c);
    // claiming pairs
    _cluster_gen(sudoku, val, row_c, box_c);
    _cluster_gen(sudoku, val, col_c, box_c);
    // row & col intersections are hidden singles
  }

  return stack_size(sudoku->decr_poss);
}
