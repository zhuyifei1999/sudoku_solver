#ifndef  COMBINATION_H_
#define COMBINATION_H_

#include "possibility.h"

void combination_possibility(int n, val_type *possibility_arr, void (*fn)(int, val_type *, void *), void *arg);
void combination_cluster(int n, cluster cluster_spec, void (*fn)(int, pos_type *, void *), void *arg);

#endif
