#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

#include <assert.h>

bool pointing_pair_plus(sudoku *sudoku) {
  bool c = false;
  _for_val(val) {
    _for_all_cells(ci, cj) {
      pos_type fi, fj;
      fi = fj = -1;
      // search if a value is unique to a row / column in a cell
      // -1: not found at all; 9: not unique
      _for_place_cell(ai, aj, ci, cj) {
        if (sudoku->arr[ai][aj] == val) break;
        if (is_val_possible(sudoku->possibilities[ai][aj], val)) {
          if (fi >= 0 && fi != ai) fi = 9;
          else fi = ai;
          if (fj >= 0 && fj != aj) fj = 9;
          else fj = aj;
        }
      }

      // if uniqueness is found, other places in the row / column outside of
      // the cell cannot have the value.

      if (fi >=0 && fi < 9) {
        bool f = false;
        _for_place_horz(ai, aj, fi, 0) {
          if (aj / 3 * 3 == cj) continue;
          f |= decr_possible(sudoku, ai, aj, val);
        }
        if (f) debug_print("horz: %d %d %d\n", fi, cj, val);
        c |= f;
      }

      if (fj >=0 && fj < 9) {
        bool f = false;
        _for_place_vert(ai, aj, 0, fj) {
          if (ai / 3 * 3 == ci) continue;
          f |= decr_possible(sudoku, ai, aj, val);
        }
        if (f) debug_print("vert: %d %d %d\n", ci, fj, val);
        c |= f;
      }
    }
  }
  return c;
}
