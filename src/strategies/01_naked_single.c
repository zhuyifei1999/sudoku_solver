#include "../strategic.h"
#include "../debug.h"

bool naked_single(sudoku *sudoku) {
  if (!sudoku->poststacksize) return false;
  while (sudoku->poststacksize) {
    pos_type pos = sudoku->poststack[--sudoku->poststacksize];
    val_type val = sudoku->possibilities[pos.i][pos.j][0];
    place(sudoku, pos, val);
    debug_print("%d %d %d\n", pos.i, pos.j, val);
  }
  return true;
}
