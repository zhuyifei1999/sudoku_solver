#include "sudoku.h"
#include "io.h"

#include <stdio.h>

void solve_sudoku(sudoku_arr sudoku);

int main(int argc, char const *argv[]) {
  sudoku_arr sudoku;
  parse_sudoku(sudoku);
  // printf("INPUT:\n");
  // print_sudoku(sudoku);
  solve_sudoku(sudoku);
  // printf("OUTPUT:\n");
  print_sudoku(sudoku);
  return 0;
}
