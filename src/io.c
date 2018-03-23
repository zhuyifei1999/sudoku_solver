#include "sudoku.h"
#include "position.h"

#include <stdio.h>
#include <stdlib.h>

void print_sudoku(val_t sudoku[9][9]) {
  for_pos_cluster_zero(col_c, initpos, ({
    for_pos_cluster(cluster(initpos, row_c), pos, ({
      if (pos.j) printf(" ");
      printf(printf_val, sudoku[pos.i][pos.j]);
    }))
    printf("\n");
  }))
}

void parse_sudoku(val_t sudoku[9][9]) {
  for_pos_cluster_zero(all_c, pos, ({
    if (scanf(printf_val, &sudoku[pos.i][pos.j]) == EOF) {
      perror(NULL);
      abort();
    }
  }))
}
