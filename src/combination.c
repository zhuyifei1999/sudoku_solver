#include "combination.h"

#include <stdlib.h>

static void _combination_possibility(poss_i_t n, poss_i_t i, val_t *target, poss_i_t current,
    val_t *possibility_arr, void (*fn)(poss_i_t, val_t *, void *), void *arg) {
  if (n == i) {
    target[i] = 0;
    return fn(n, target, arg);
  }
  val_t value;
  for (;(value = possibility_arr[current]); current++) {
    target[i] = value;
    _combination_possibility(n, i + 1, target, current, possibility_arr, fn, arg);
  }
}

void combination_possibility(poss_i_t n, val_t *possibility_arr, void (*fn)(poss_i_t, val_t *, void *), void *arg) {
  val_t *target = (val_t *)malloc((n+1)*sizeof(val_t));
  _combination_possibility(n, 0, target, 0, possibility_arr, fn, arg);
  free(target);
}

static void _combination_cluster(poss_i_t n, poss_i_t i, pos_t *target, pos_t current,
    cluster cluster_spec, void (*fn)(poss_i_t, pos_t *, void *), void *arg) {
  if (n == i) {
    return fn(n, target, arg);
  }
  if (i) {
    if (!(*cluster_spec.gen->next)(&current)) return;
  } else {
    (*cluster_spec.gen->init)(&current, cluster_spec.rel);
  }
  do {
    target[i] = current;
    _combination_cluster(n, i + 1, target, current, cluster_spec, fn, arg);
  } while ((*cluster_spec.gen->next)(&current));
}

void combination_cluster(poss_i_t n, cluster cluster_spec, void (*fn)(poss_i_t, pos_t *, void *), void *arg) {
  pos_t *target = (pos_t *)malloc((n+1)*sizeof(pos_t));
  pos_t current;
  _combination_cluster(n, 0, target, current, cluster_spec, fn, arg);
  free(target);
}
