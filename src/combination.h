#ifndef  COMBINATION_H_
#define COMBINATION_H_

#include "possibility.h"

void combination_possibility(poss_i_t n, val_t *possibility_arr, void (*fn)(poss_i_t, val_t *, void *), void *arg);
void combination_cluster(poss_i_t n, cluster cluster_spec, void (*fn)(pos_s_t, pos_t *, void *), void *arg);

#endif
