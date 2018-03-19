#include "sudoku.h"

#include <stddef.h>

static void _p_vert_init(pos_type *target, pos_type src) {
  target->i = 0; target->j = src.j;
}
static bool _p_vert_next(pos_type *target) {
  return ++target->i < 9;
}
static const struct cluster vert_ic = {
  .rel = { .i = 0, .j = 0 },
  .gen = &horz_c,
};
cluster_gen vert_c = {
  .init = &_p_vert_init,
  .next = &_p_vert_next,
  .complement = &vert_ic,
};

static void _p_horz_init(pos_type *target, const pos_type src) {
  target->i = src.i; target->j = 0;
}
static bool _p_horz_next(pos_type *target) {
  return ++target->j < 9;
}
static const struct cluster horz_ic = {
  .rel = zero_pos,
  .gen = &vert_c,
};
cluster_gen horz_c = {
  .init = &_p_horz_init,
  .next = &_p_horz_next,
  .complement = &horz_ic,
};

static void _p_cellgen_init(pos_type *target, const pos_type src) {
  target->i = target->j = 0;
}
static bool _p_cellgen_next(pos_type *target) {
  return (target->j += 3) % 9 ? 1 : (target->j -= 9, (target->i += 3) % 9);
}
static cluster_gen cellgen_c = {
  .init = &_p_cellgen_init,
  .next = &_p_cellgen_next,
  .complement = NULL,
};
static void _p_cell_init(pos_type *target, const pos_type src) {
  target->i = src.i / 3 * 3; target->j = src.j / 3 * 3;
}
static bool _p_cell_next(pos_type *target) {
  return ++target->j % 3 ? 1 : (target->j -= 3, ++target->i % 3);
}
static struct cluster cell_ic = {
  .rel = zero_pos,
  .gen = &cellgen_c,
};
cluster_gen cell_c = {
  .init = &_p_cell_init,
  .next = &_p_cell_next,
  .complement = &cell_ic,
};

static void _p_all_init(pos_type *target, const pos_type src) {
  target->i = target->j = 0;
}
static bool _p_all_next(pos_type *target) {
  return ++target->j % 9 ? 1 : (target->j -= 9, ++target->i % 9);
}
cluster_gen all_c = {
  .init = &_p_all_init,
  .next = &_p_all_next,
  .complement = NULL,
};
