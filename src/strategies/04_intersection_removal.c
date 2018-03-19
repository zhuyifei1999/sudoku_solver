#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static bool _cluster(sudoku *sudoku, val_t val,
  cluster cluster_pre, cluster_gen gen_rm
) {
  cluster *cluster_rm = NULL;
  unsigned short num_found = 0;

#define do_return(value) do { if (cluster_rm) free(cluster_rm); return value; } while (0)

  // The value should only belong to one cluster of gen_rm
  for_pos_cluster(c, cluster_pre, pos, ({
    if (sudoku->arr[pos.i][pos.j] == val) do_return(false);

    num_found++;

    if (is_val_possible(sudoku->possibilities[pos.i][pos.j], val)) {
      if (cluster_rm) {
        if (!is_pos_cluster(pos, *cluster_rm)) do_return(false);
      } else {
        // Terrible hack :(
        cluster cluster_rm_init = cluster(pos, gen_rm);
        cluster_rm = (struct cluster *)malloc(sizeof(struct cluster));
        memcpy(cluster_rm, &cluster_rm_init, sizeof(struct cluster));
      }
    }
  }))

  if (!cluster_rm) do_return(false);

  // num_found = 1 is a naked single
  assert(num_found > 1);

  bool f = false;
  // any other clusters of gen_pre in cluster_rm cannot have the value
  for_pos_cluster(c, *cluster_rm, pos, ({
    if (is_pos_cluster(pos, cluster_pre)) continue;
    f |= decr_possible(sudoku, pos, val);
  }))

  if (f) {
    debug_print(
      "%s (" printf_pos_s ", " printf_pos_s ") "
      "%s (" printf_pos_s ", " printf_pos_s ") "
      printf_val "\n",
      cluster_pre.gen->name, cluster_pre.rel.i, cluster_pre.rel.j,
      cluster_rm->gen->name, cluster_rm->rel.i, cluster_rm->rel.j,
      val
    );
  }

  do_return(f);
}

static bool _cluster_gen(sudoku *sudoku, val_t val,
  cluster_gen gen_pre, cluster_gen gen_rm
) {
  bool f = false;

  for_pos_cluster(initc, *gen_pre.complement, initpos, ({
    f |= _cluster(sudoku, val, cluster(initpos, gen_pre), gen_rm);
  }))

  return f;
}

bool intersection_removal(sudoku *sudoku) {
  bool f = false;
  for_val(val) {
    // pointing pairs
    f |= _cluster_gen(sudoku, val, cell_c, horz_c);
    f |= _cluster_gen(sudoku, val, cell_c, vert_c);
    // claiming pairs
    f |= _cluster_gen(sudoku, val, horz_c, cell_c);
    f |= _cluster_gen(sudoku, val, vert_c, cell_c);
    // horz & vert intersections are hidden singles
  }
  return f;
}
