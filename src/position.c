#include "sudoku.h"

#include <stddef.h>

static void _p_col_init(pos_t *target, pos_t src) {
  target->i = 0; target->j = src.j;
}
static bool _p_col_next(pos_t *target) {
  return ++target->i < 9;
}
static const cluster_t col_ic = {
  .rel = { .i = 0, .j = 0 },
  .gen = &row_c,
};
cluster_gen_t col_c = {
  .init = &_p_col_init,
  .next = &_p_col_next,
  .complement = &col_ic,
  .name = "col",
};

static void _p_row_init(pos_t *target, const pos_t src) {
  target->i = src.i; target->j = 0;
}
static bool _p_row_next(pos_t *target) {
  return ++target->j < 9;
}
static const cluster_t row_ic = {
  .rel = zero_pos,
  .gen = &col_c,
};
cluster_gen_t row_c = {
  .init = &_p_row_init,
  .next = &_p_row_next,
  .complement = &row_ic,
  .name = "row",
};

static void _p_boxgen_init(pos_t *target, const pos_t src) {
  target->i = target->j = 0;
}
static bool _p_boxgen_next(pos_t *target) {
  return (target->j += 3) % 9 ? 1 : (target->j -= 9, (target->i += 3) % 9);
}
static cluster_gen_t boxgen_c = {
  .init = &_p_boxgen_init,
  .next = &_p_boxgen_next,
  .complement = NULL,
  .name = NULL,
};
static void _p_box_init(pos_t *target, const pos_t src) {
  target->i = src.i / 3 * 3; target->j = src.j / 3 * 3;
}
static bool _p_box_next(pos_t *target) {
  return ++target->j % 3 ? 1 : (target->j -= 3, ++target->i % 3);
}
static cluster_t box_ic = {
  .rel = zero_pos,
  .gen = &boxgen_c,
};
cluster_gen_t box_c = {
  .init = &_p_box_init,
  .next = &_p_box_next,
  .complement = &box_ic,
  .name = "box",
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
