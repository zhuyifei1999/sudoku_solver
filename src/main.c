#include "sudoku.h"
#include "io.h"

#include <stdio.h>

void solve_sudoku(val_type sudoku[9][9]);

int main(int argc, char const *argv[]) {
  val_type sudoku[9][9];
  parse_sudoku(sudoku);
  // printf("INPUT:\n");
  // print_sudoku(sudoku);
  solve_sudoku(sudoku);
  // printf("OUTPUT:\n");
  print_sudoku(sudoku);
  return 0;
}
