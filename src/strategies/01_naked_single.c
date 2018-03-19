#include "../strategic.h"
#include "../debug.h"

bool naked_single(sudoku *sudoku) {
  if (!sudoku->poststacksize) return false;
  while (sudoku->poststacksize) {
    pos_t pos = sudoku->poststack[--sudoku->poststacksize];
    val_t val = sudoku->possibilities[pos.i][pos.j][0];
    place(sudoku, pos, val);
    debug_print("(" printf_pos_s ", " printf_pos_s ") " printf_val "\n", pos.i, pos.j, val);
  }
  return true;
}
