#include "sudoku.h"

#include <stddef.h>

static void _p_vert_init(pos_t *target, pos_t src) {
  target->i = 0; target->j = src.j;
}
static bool _p_vert_next(pos_t *target) {
  return ++target->i < 9;
}
static const cluster_t vert_ic = {
  .rel = { .i = 0, .j = 0 },
  .gen = &horz_c,
};
cluster_gen_t vert_c = {
  .init = &_p_vert_init,
  .next = &_p_vert_next,
  .complement = &vert_ic,
  .name = "vert",
};

static void _p_horz_init(pos_t *target, const pos_t src) {
  target->i = src.i; target->j = 0;
}
static bool _p_horz_next(pos_t *target) {
  return ++target->j < 9;
}
static const cluster_t horz_ic = {
  .rel = zero_pos,
  .gen = &vert_c,
};
cluster_gen_t horz_c = {
  .init = &_p_horz_init,
  .next = &_p_horz_next,
  .complement = &horz_ic,
  .name = "horz",
};

static void _p_cellgen_init(pos_t *target, const pos_t src) {
  target->i = target->j = 0;
}
static bool _p_cellgen_next(pos_t *target) {
  return (target->j += 3) % 9 ? 1 : (target->j -= 9, (target->i += 3) % 9);
}
static cluster_gen_t cellgen_c = {
  .init = &_p_cellgen_init,
  .next = &_p_cellgen_next,
  .complement = NULL,
  .name = NULL,
};
static void _p_cell_init(pos_t *target, const pos_t src) {
  target->i = src.i / 3 * 3; target->j = src.j / 3 * 3;
}
static bool _p_cell_next(pos_t *target) {
  return ++target->j % 3 ? 1 : (target->j -= 3, ++target->i % 3);
}
static cluster_t cell_ic = {
  .rel = zero_pos,
  .gen = &cellgen_c,
};
cluster_gen_t cell_c = {
  .init = &_p_cell_init,
  .next = &_p_cell_next,
  .complement = &cell_ic,
  .name = "cell",
};

static void _p_all_init(pos_t *target, const pos_t src) {
  target->i = target->j = 0;
}
static bool _p_all_next(pos_t *target) {
  return ++target->j % 9 ? 1 : (target->j -= 9, ++target->i % 9);
}
cluster_gen_t all_c = {
  .init = &_p_all_init,
  .next = &_p_all_next,
  .complement = NULL,
  .name = "all",
};

bool is_pos_cluster(pos_t pos, cluster_t cluster) {
  pos_t pos1, pos2;
  (*cluster.gen->init)(&pos1, pos);
  (*cluster.gen->init)(&pos2, cluster.rel);
  return is_pos_equal(pos1, pos2);
}

bool is_cluster_equal(cluster_t x, cluster_t y) {
  if (x.gen != y.gen) return false;
  pos_t pos1, pos2;
  (*x.gen->init)(&pos1, x.rel);
  (*y.gen->init)(&pos2, y.rel);
  return is_pos_equal(pos1, pos2);
}
