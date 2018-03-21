#include "../strategic.h"
#include "../debug.h"

bool naked_single(sudoku_t *sudoku) {
  if (!stack_size(sudoku->ns_pos)) return false;
  while (stack_size(sudoku->ns_pos)) {
    pos_t pos = stack_pop(sudoku->ns_pos);
    val_t val = sudoku->possibilities[pos.i][pos.j][0];
    place(sudoku, pos, val);
    debug_print(printf_pos " " printf_val, pos.i, pos.j, val);
  }
  return true;
}
