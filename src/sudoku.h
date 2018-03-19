#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <stdbool.h>

#include "position.h"

typedef unsigned int val_type;
typedef val_type sudoku_arr[9][9];

#define for_val(val) for (val_type val = 1; val <= 9; val++)

#endif
