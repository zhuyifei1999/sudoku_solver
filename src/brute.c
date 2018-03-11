#include "brute.h"

bool _is_val_valid(val_type sudoku[9][9], pos_type i, pos_type j, val_type val) {
  _for_place_vert(ai, aj, i, j) {
    if (sudoku[ai][aj] == val) return false;
  }
  _for_place_horz(ai, aj, i, j) {
    if (sudoku[ai][aj] == val) return false;
  }
  _for_place_cell(ai, aj, i, j) {
    if (sudoku[ai][aj] == val) return false;
  }

  return true;
}

bool _solve_sudoku(val_type sudoku[9][9], pos_type c) {
  for (pos_type a = c; a < 81; a++) {
    pos_type i = a / 9, j = a % 9;
    if (sudoku[i][j]) continue;

    // attempt every single value
    val_type val;
    for (val = 1; val <= 9; val++) {
      if (_is_val_valid(sudoku, i, j, val)) {
        sudoku[i][j] = val;
        if (_solve_sudoku(sudoku, a + 1)) return 1;
      }
    }
    // uh oh... no valid and successful value found
    if (val > 9) sudoku[i][j] = 0;
    return 0;
  }
  return 1;
}

void solve_sudoku(val_type sudoku[9][9]) {
  _solve_sudoku(sudoku, 0);
}
