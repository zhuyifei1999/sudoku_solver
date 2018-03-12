#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

#include <assert.h>

bool _naked_pair_cluster(sudoku *sudoku, pos_type i, pos_type j, val_type *poss, pos_type size,
  void (*init)(pos_type *, pos_type *, pos_type, pos_type),
  bool (*next)(pos_type *, pos_type *)
) {
  pos_type s = size;

  pos_type ai, aj;
  (*init)(&ai, &aj, i, j);
  do {
    // find # of exclusion
    if (is_exact_same_poss(sudoku->possibilities[ai][aj], poss)) s--;
  } while ((*next)(&ai, &aj));

  assert(s >= 0);
  if (s) return false;

  bool c = false;
  (*init)(&ai, &aj, i, j);
  do {
    // not exclusion => decr
    if (!is_exact_same_poss(sudoku->possibilities[ai][aj], poss)) {
      pos_type post;
      val_type val;
      for (post = 0; (val = poss[post]); post++) {
        c |= decr_possible(sudoku, ai, aj, val);
      }
    }
  } while ((*next)(&ai, &aj));
  return c;
}


bool naked_pair_plus(sudoku *sudoku) {
  bool f = false;
  _for_all_places(i, j) {
    if (sudoku->arr[i][j]) continue;

    val_type *poss = sudoku->possibilities[i][j];
    pos_type size = poss_size(poss);

    bool c = false;

    c |= _naked_pair_cluster(sudoku, i, j, poss, size, &_place_vert_gen_init, &_place_vert_gen_next);
    c |= _naked_pair_cluster(sudoku, i, j, poss, size, &_place_horz_gen_init, &_place_horz_gen_next);
    c |= _naked_pair_cluster(sudoku, i, j, poss, size, &_place_cell_gen_init, &_place_cell_gen_next);

    if (c) debug_print("%d %d\n", i, j);

    f |= c;
  }
  return f;
}
