#ifndef POSSIBILITY_H_
#define POSSIBILITY_H_

#include "sudoku.h"

bool is_val_possible(val_type *possibilities, val_type possibility);
bool is_subset_poss(val_type *pool, val_type *subset);
bool is_exact_same_poss(val_type *a, val_type *b);
int poss_size(val_type *possibilities);

#endif
