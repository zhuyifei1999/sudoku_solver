#include "brute.h"

bool _is_val_valid(val_type sudoku[9][9], pos_type position, val_type val) {
  for_pos_cluster(c, cluster(position, vert_c), pos, ({
    if (sudoku[pos.i][pos.j] == val) return false;
  }))
  for_pos_cluster(c, cluster(position, horz_c), pos, ({
    if (sudoku[pos.i][pos.j] == val) return false;
  }))
  for_pos_cluster(c, cluster(position, cell_c), pos, ({
    if (sudoku[pos.i][pos.j] == val) return false;
  }))

  return true;
}

bool _solve_sudoku(sudoku_arr sudoku, int c) {
  for (int a = c; a < 81; a++) {
    pos_type pos = { .i = a / 9, .j = a % 9 };
    if (sudoku[pos.i][pos.j]) continue;

    // attempt every single value
    val_type val;
    for (val = 1; val <= 9; val++) {
      if (_is_val_valid(sudoku, pos, val)) {
        sudoku[pos.i][pos.j] = val;
        if (_solve_sudoku(sudoku, a + 1)) return true;
      }
    }
    // uh oh... no valid and successful value found
    if (val > 9) sudoku[pos.i][pos.j] = 0;
    return false;
  }
  return true;
}

void solve_sudoku(sudoku_arr sudoku) {
  _solve_sudoku(sudoku, 0);
}
