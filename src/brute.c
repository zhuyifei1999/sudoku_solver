#include "brute.h"

static bool _is_val_valid(val_t sudoku[9][9], pos_t position, val_t val) {
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

static bool _solve_sudoku(sudoku_arr sudoku, pos_t pos) {
  do {
    if (sudoku[pos.i][pos.j]) continue;

    // attempt every single value
    for_val(val) {
      if (_is_val_valid(sudoku, pos, val)) {
        sudoku[pos.i][pos.j] = val;
        pos_t nextpos = pos;
        if (!(*all_c.next)(&nextpos)) return true; // reached the end
        if (_solve_sudoku(sudoku, nextpos)) return true;
      }
    }
    // uh oh... no valid and successful value found
    sudoku[pos.i][pos.j] = 0;
    return false;
  } while ((*all_c.next)(&pos));
  return true;
}

void solve_sudoku(sudoku_arr sudoku) {
  pos_t position = zero_pos;
  _solve_sudoku(sudoku, position);
}
