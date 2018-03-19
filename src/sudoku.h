#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <stdbool.h>

#include "position.h"

typedef unsigned char val_t; // value 1-9, 0 is unknown / invalid

#define printf_val "%u"

typedef val_t sudoku_arr[9][9];

#define for_val(val) for (val_t val = 1; val <= 9; val++)

#endif
