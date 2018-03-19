#include "combination.h"

#include <stdlib.h>

static void _combination_possibility(int n, int i, val_type *target, int current,
    val_type *possibility_arr, void (*fn)(int, val_type *, void *), void *arg) {
  if (n == i) {
    target[i] = 0;
    return fn(n, target, arg);
  }
  val_type value;
  for (;(value = possibility_arr[current]); current++) {
    target[i] = value;
    _combination_possibility(n, i + 1, target, current, possibility_arr, fn, arg);
  }
}

void combination_possibility(int n, val_type *possibility_arr, void (*fn)(int, val_type *, void *), void *arg) {
  val_type *target = (val_type *)malloc((n+1)*sizeof(val_type));
  _combination_possibility(n, 0, target, 0, possibility_arr, fn, arg);
  free(target);
}

static void _combination_cluster(int n, int i, pos_type *target, pos_type current,
    cluster cluster_spec, void (*fn)(int, pos_type *, void *), void *arg) {
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

void combination_cluster(int n, cluster cluster_spec, void (*fn)(int, pos_type *, void *), void *arg) {
  pos_type *target = (pos_type *)malloc((n+1)*sizeof(pos_type));
  pos_type current;
  _combination_cluster(n, 0, target, current, cluster_spec, fn, arg);
  free(target);
}
