#include "../strategic.h"
#include "../debug.h"

bool naked_single(sudoku *sudoku) {
  if (!sudoku->poststacksize) return false;
  while (sudoku->poststacksize) {
    pos_type j = sudoku->poststack[--sudoku->poststacksize];
    pos_type i = sudoku->poststack[--sudoku->poststacksize];
    val_type val = sudoku->possibilities[i][j][0];
    place(sudoku, i, j, val);
    debug_print("%d %d %d\n", i, j, val);
  }
  return true;
}
