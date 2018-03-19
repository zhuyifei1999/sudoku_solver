#ifndef POSITION_H_
#define POSITION_H_

typedef int pos_single_type;

typedef struct pos_type {
  pos_single_type i;
  pos_single_type j;
} pos_type;

#define zero_pos { .i = 0, .j = 0 }

typedef struct cluster_gen {
  void (*init)(pos_type *target, const pos_type src);
  bool (*next)(pos_type *target);
  const struct cluster *complement;
  const char *name;
} cluster_gen;

extern cluster_gen vert_c, horz_c, cell_c, all_c;

typedef struct cluster {
  const pos_type rel;
  const struct cluster_gen *gen;
} cluster;

#define cluster(pos_src, cluster_gen) { .rel = pos_src, .gen = &cluster_gen }

#define for_pos_cluster(cluster_var, cluster_exp, position_var, body) { \
  cluster cluster_var = cluster_exp; \
  pos_type position_var; \
  (*cluster_var.gen->init)(&position_var, cluster_var.rel); \
  do { \
    body; \
  } while ((*cluster_var.gen->next)(&position_var)); \
};

#define for_pos_cluster_zero(cluster_var, cluster_gen, position_var, body) \
  for_pos_cluster(cluster_var, cluster(zero_pos, cluster_gen), position_var, body)

#define is_pos_equal(x, y) (x.i == y.i && x.j == y.j)

#endif
