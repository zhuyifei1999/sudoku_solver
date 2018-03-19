#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

static bool _cluster(sudoku *sudoku, pos_type position, val_type val, cluster_gen gen) {
  for_pos_cluster(c, cluster(position, gen), pos, ({
    if (_is_pos_equal(position, pos)) continue;
    if (is_val_possible(sudoku->possibilities[pos.i][pos.j], val)) {
      return false;
    }
  }))
  return true;
}

bool _is_hidden_single(sudoku *sudoku, pos_type position, val_type val) {
  return _cluster(sudoku, position, val, vert_c) ||
    _cluster(sudoku, position, val, horz_c) ||
    _cluster(sudoku, position, val, cell_c);
}

bool hidden_single(sudoku *sudoku) {
  bool f = false;
  for_val(val) {
    for_pos_cluster_zero(c, all_c, pos, ({
      if (sudoku->arr[pos.i][pos.j]) continue;
      if (!is_val_possible(sudoku->possibilities[pos.i][pos.j], val)) continue;

      if (!_is_hidden_single(sudoku, pos, val)) continue;

      place(sudoku, pos, val);
      debug_print("%d %d %d\n", pos.i, pos.j, val);
      f = true;
    }))
  }
  return f;
}
