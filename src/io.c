#include "possibility.h"

#include <stdio.h>
#include <stdlib.h>

void print_sudoku(val_t sudoku[9][9]) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (j) printf(" ");
      printf("%d", sudoku[i][j]);
    }
    printf("\n");
  }
}

void parse_sudoku(val_t sudoku[9][9]) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (scanf("%d", &sudoku[i][j]) == EOF) {
        perror(NULL);
        abort();
      }
    }
  }
}
