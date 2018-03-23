#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

static bool _cluster(sudoku_t *sudoku, pos_t position, val_t val, cluster_gen_t gen) {
  // true if position is the only polce that have a possibility of val in the cluster
  for_pos_cluster(cluster(position, gen), pos, ({
    if (is_pos_equal(position, pos)) continue;
    if (is_val_possible(sudoku->possibilities[pos.i][pos.j], val)) {
      return false;
    }
  }))
  return true;
}

bool _is_hidden_single(sudoku_t *sudoku, pos_t position, val_t val) {
  return _cluster(sudoku, position, val, col_c) ||
         _cluster(sudoku, position, val, row_c) ||
         _cluster(sudoku, position, val, box_c);
}

STRATEGY("Hidden Single", 2) {
  bool f = false;
  for_val(val) {
    for_pos_cluster_zero(all_c, pos, ({
      if (sudoku->arr[pos.i][pos.j]) continue;
      if (!is_val_possible(sudoku->possibilities[pos.i][pos.j], val)) continue;

      if (!_is_hidden_single(sudoku, pos, val)) continue;

      place(sudoku, pos, val);
      debug_print(printf_pos " " printf_val, pos.i, pos.j, val);
      f = true;
    }))
  }
  return f;
}
