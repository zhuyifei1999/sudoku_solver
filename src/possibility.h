#ifndef POSSIBILITY_H_
#define POSSIBILITY_H_

#include "sudoku.h"

#include <sys/types.h>

typedef size_t poss_i_t; // index of possibilities array
typedef ssize_t signed_poss_i_t;

#define printf_poss_i "%zd"

bool is_val_possible(val_t *possibilities, val_t possibility);
bool is_subset_poss(val_t *pool, val_t *subset);
bool is_exact_same_poss(val_t *a, val_t *b);
bool has_intersection_poss(val_t *a, val_t *b);
poss_i_t poss_size(val_t *possibilities);
void copy_poss(val_t *src, val_t *target);

#endif
