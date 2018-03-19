#ifndef POSITION_H_
#define POSITION_H_

#include <stddef.h>

typedef size_t pos_s_t; // single index of position

#define printf_pos_s "%zd"

typedef struct pos_t {
  pos_s_t i;
  pos_s_t j;
} pos_t;

#define zero_pos { .i = 0, .j = 0 }

typedef struct cluster_gen_t {
  void (*init)(pos_t *target, const pos_t src);
  bool (*next)(pos_t *target);
  const struct cluster_t *complement;
  const char *name;
} cluster_gen_t;

extern cluster_gen_t vert_c, horz_c, cell_c, all_c;

typedef struct cluster_t {
  const pos_t rel;
  const struct cluster_gen_t *gen;
} cluster_t;

#define cluster(pos_src, cluster_gen) (cluster_t){ .rel = pos_src, .gen = &cluster_gen }

#define for_pos_cluster(cluster_var, cluster_exp, position_var, body) { \
  cluster_t cluster_var = cluster_exp; \
  pos_t position_var; \
  (*cluster_var.gen->init)(&position_var, cluster_var.rel); \
  do { \
    body; \
  } while ((*cluster_var.gen->next)(&position_var)); \
};

#define for_pos_cluster_zero(cluster_var, cluster_gen, position_var, body) \
  for_pos_cluster(cluster_var, cluster(zero_pos, cluster_gen), position_var, body)

#define is_pos_equal(x, y) (x.i == y.i && x.j == y.j)
bool is_pos_cluster(pos_t pos, cluster_t cluster);

#endif
