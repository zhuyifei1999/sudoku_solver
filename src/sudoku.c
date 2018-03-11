#include "sudoku.h"

void _place_vert_gen_init(pos_type *i_targ, pos_type *j_targ, pos_type i_src, pos_type j_src) {
  *i_targ = 0; *j_targ = j_src;
}
void _place_horz_gen_init(pos_type *i_targ, pos_type *j_targ, pos_type i_src, pos_type j_src) {
  *i_targ = i_src; *j_targ = 0;
}
void _place_cell_gen_init(pos_type *i_targ, pos_type *j_targ, pos_type i_src, pos_type j_src) {
  *i_targ = i_src / 3 * 3; *j_targ = j_src / 3 * 3;
}
bool _place_vert_gen_next(pos_type *i_targ, pos_type *j_targ) {
  return ++(*i_targ) < 9;
}
bool _place_horz_gen_next(pos_type *i_targ, pos_type *j_targ) {
  return ++(*j_targ) < 9;
}
bool _place_cell_gen_next(pos_type *i_targ, pos_type *j_targ) {
  return ++(*j_targ) % 3 ? 1 : (*j_targ -= 3, ++(*i_targ) % 3);
}
