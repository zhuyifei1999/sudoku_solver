#include "possibility.h"

bool is_val_possible(val_t *possibilities, val_t possibility) {
  poss_i_t post;
  val_t val;
  for (post = 0; (val = possibilities[post]); post++) {
    if (val == possibility) return true;
  }
  return false;
}

bool is_subset_poss(val_t *pool, val_t *subset) {
  val_t post, val;
  for (post = 0; (val = subset[post]); post++) {
    if (!is_val_possible(pool, val)) return false;
  }
  return true;
}

bool is_exact_same_poss(val_t *a, val_t *b) {
  return is_subset_poss(a, b) && is_subset_poss(b, a);
}

poss_i_t poss_size(val_t *possibilities) {
  poss_i_t post;
  for (post = 0; possibilities[post]; post++) {}
  return post;
}

void copy_poss(val_t *src, val_t *target) {
  // TODO: optimize this
  val_t post, val;
  for (post = 0; (val = src[post]); post++) {
    if (!is_val_possible(target, val)) {
      poss_i_t size = poss_size(target);
      target[size] = val;
      target[size+1] = 0;
    }
  }
}
