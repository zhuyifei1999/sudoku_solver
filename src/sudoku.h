#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <stdbool.h>

typedef unsigned int val_type;
typedef unsigned int pos_type;

#define _for_val(val) for (val_type val = 1; val <= 9; val++)
#define _for_all_places(i_src, j_src) for (pos_type i_src = 0, j_src = 0; \
  i_src < 9; ++j_src < 9 ? 1 : (j_src = 0, i_src += 1))
#define _for_all_cells(i_src, j_src) for (pos_type i_src = 0, j_src = 0; \
  i_src < 9; j_src < 6 ? j_src += 3: (j_src = 0, i_src += 3))

/* The following should produce the same pairs of positions:
 * pos_type i_targ, j_targ;
 * _place_vert_gen_init(&i_targ, &j_targ, i_src, j_src);
 * do {
 *     decr_poss(sudoku, i_targ, j_targ, val);
 * } while (_place_vert_gen_next(&i_targ, &j_targ));
 * --- AND ---
 * _for_place_vert(i_targ, j_targ, i_src, j_src) {
 *     decr_poss(sudoku, i_targ, j_targ, val);
 * }
 * Use whichever that is more suitable.
 */

#define _for_place_vert(i_targ, j_targ, i_src, j_src) \
  for (pos_type i_targ = 0, j_targ = j_src; i_targ < 9; i_targ++)
#define _for_place_horz(i_targ, j_targ, i_src, j_src) \
  for (pos_type i_targ = i_src, j_targ = 0; j_targ < 9; j_targ++)
#define _for_place_cell(i_targ, j_targ, i_src, j_src) \
  for (pos_type i_targ = i_src / 3 * 3, j_targ = j_src / 3 * 3; \
    i_targ % 3 != 0 || j_targ % 3 != 0 || i_targ / 3 == i_src / 3; \
    ++j_targ % 3 ? 1 : (j_targ -= 3, i_targ++))

void _place_vert_gen_init(pos_type *i_targ, pos_type *j_targ, pos_type i_src, pos_type j_src);
void _place_horz_gen_init(pos_type *i_targ, pos_type *j_targ, pos_type i_src, pos_type j_src);
void _place_cell_gen_init(pos_type *i_targ, pos_type *j_targ, pos_type i_src, pos_type j_src);
bool _place_vert_gen_next(pos_type *i_targ, pos_type *j_targ);
bool _place_horz_gen_next(pos_type *i_targ, pos_type *j_targ);
bool _place_cell_gen_next(pos_type *i_targ, pos_type *j_targ);

#endif
