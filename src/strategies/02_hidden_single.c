#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

bool _is_hidden_single(sudoku *sudoku, pos_type i, pos_type j, val_type val) {
  bool f;

  f = false;
  _for_place_vert(ai, aj, i, j) {
    if (ai == i) continue;
    if (is_val_possible(sudoku->possibilities[ai][aj], val)) {
      f = true;
    }
  }
  if (!f) return true;

  f = false;
  _for_place_horz(ai, aj, i, j) {
    if (aj == j) continue;
    if (is_val_possible(sudoku->possibilities[ai][aj], val)) {
      f = true;
    }
  }
  if (!f) return true;

  f = false;
  _for_place_cell(ai, aj, i, j) {
    if (ai == i && aj == j) continue;
    if (is_val_possible(sudoku->possibilities[ai][aj], val)) {
      f = true;
    }
  }
  if (!f) return true;

  return false;
}

bool hidden_single(sudoku *sudoku) {
  int c = false;
  _for_val(val) {
    _for_all_places(i, j) {
      if (sudoku->arr[i][j]) continue;
      if (!is_val_possible(sudoku->possibilities[i][j], val)) continue;

      if (!_is_hidden_single(sudoku, i, j, val)) continue;

      debug_print("%d %d %d\n", i, j, val);
      place(sudoku, i, j, val);
      c = true;
    }
  }
  return c;
}
