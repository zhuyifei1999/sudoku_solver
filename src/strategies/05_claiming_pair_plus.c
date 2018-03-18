#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

#include <assert.h>


bool claiming_pair_plus(sudoku *sudoku) {
  bool c = false;
  _for_val(val) {
    _for_place_horz(i, j, 0, 0) {
      UNUSED(i);
      pos_type fi = -1;

      // search if a value is unique to a cell in a row / column
      // -1: not found at all; 4: not unique
      _for_place_vert(ai, aj, 0, j) {
        if (sudoku->arr[ai][aj] == val) break;
        if (is_val_possible(sudoku->possibilities[ai][aj], val)) {
          if (fi >= 0 && fi != ai / 3) fi = 4;
          else fi = ai / 3;
        }
      }

      // if uniqueness is found, other places in the cell outside of
      // the row / column cannot have the value.
      if (fi >=0 && fi < 4) {
        bool f = false;
        _for_place_cell(ai, aj, fi * 3, j) {
          if (aj == j) continue;
          f |= decr_possible(sudoku, ai, aj, val);
        }
        if (f) debug_print("vert: %d %d %d\n", fi * 3, j, val);
        c |= f;
      }
    }

    _for_place_vert(i, j, 0, 0) {
      UNUSED(j);
      pos_type fj = -1;

      // ditto
      _for_place_horz(ai, aj, i, j) {
        if (sudoku->arr[ai][aj] == val) break;
        if (is_val_possible(sudoku->possibilities[ai][aj], val)) {
          if (fj >= 0 && fj != aj / 3) fj = 4;
          else fj = aj / 3;
        }
      }

      // ditto
      if (fj >=0 && fj < 4) {
        bool f = false;
        _for_place_cell(ai, aj, i, fj * 3) {
          if (ai == i) continue;
          f |= decr_possible(sudoku, ai, aj, val);
        }
        if (f) debug_print("horz: %d %d %d\n", i, fj * 3, val);
        c |= f;
      }
    }
  }
  return c;
}
