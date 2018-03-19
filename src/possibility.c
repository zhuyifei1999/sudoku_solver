#include "possibility.h"

bool is_val_possible(val_type *possibilities, val_type possibility) {
  int post;
  val_type val;
  for (post = 0; (val = possibilities[post]); post++) {
    if (val == possibility) return true;
  }
  return false;
}

bool is_subset_poss(val_type *pool, val_type *subset) {
  val_type post, val;
  for (post = 0; (val = subset[post]); post++) {
    if (!is_val_possible(pool, val)) return false;
  }
  return true;
}

bool is_exact_same_poss(val_type *a, val_type *b) {
  return is_subset_poss(a, b) && is_subset_poss(b, a);
}

int poss_size(val_type *possibilities) {
  int post;
  for (post = 0; possibilities[post]; post++) {}
  return post;
}

void copy_poss(val_type *src, val_type *target) {
  // TODO: optimize this
  val_type post, val;
  for (post = 0; (val = src[post]); post++) {
    if (!is_val_possible(target, val)) {
      int size = poss_size(target);
      target[size] = val;
      target[size+1] = 0;
    }
  }
}
