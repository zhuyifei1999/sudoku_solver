#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"

#include <assert.h>

static bool _ABC(val_t AB[2], val_t BC[2], val_t *A, val_t *B, val_t *C) {
  // get A, B, C from arrays of AB & BC. true if success and A != C
  val_t AB_1 = AB[0], AB_2 = AB[1],
    BC_1 = BC[0], BC_2 = BC[1];

  if (AB_1 == BC_1) {
    *B = AB_1;
    *A = AB_2;
    *C = BC_2;
  } else if (AB_1 == BC_2) {
    *B = AB_1;
    *A = AB_2;
    *C = BC_1;
  } else if (AB_2 == BC_1) {
    *B = AB_2;
    *A = AB_1;
    *C = BC_2;
  } else if (AB_2 == BC_2) {
    *B = AB_2;
    *A = AB_1;
    *C = BC_1;
  } else {
    return false;
  }

  return *A != *C;
}

static void _c_rm_from_BC(sudoku_t *sudoku, pos_t AB, pos_t BC, pos_t AC,
    cluster_t CD_c, val_t A, val_t B, val_t C) {
  // anywhere (CD) that can see both BC and AC cannot have C
  for_pos_cluster(CD_c, CD, ({
    if (is_pos_equal(CD, AB) || is_pos_equal(CD, BC) || is_pos_equal(CD, AC)) continue;
    if (sudoku->arr[CD.i][CD.j]) continue;
    if (!is_val_possible(sudoku->possibilities[CD.i][CD.j], C)) continue;

    // check CD can see AC
    if (!(is_pos_cluster(CD, cluster(AC, col_c)) ||
          is_pos_cluster(CD, cluster(AC, row_c)) ||
          is_pos_cluster(CD, cluster(AC, box_c))
    )) continue;

    assert(decr_possible(sudoku, CD, C)); // we confirmed the possibility
    debug_print(printf_pos "; AB: " printf_pos "; BC: " printf_pos "; AC: " printf_pos
      "; A: " printf_val "; B: " printf_val "; C: " printf_val,
      CD.i, CD.j, AB.i, AB.j, BC.i, BC.j, AC.i, AC.j, A, B, C);
  }))
}

static void _ac_cluster(sudoku_t *sudoku, pos_t AB, cluster_t BC_c,
    pos_t BC, cluster_t AC_c, val_t ABC_A, val_t ABC_B, val_t ABC_C) {
  // find a AC that AB can see, and contains only two possibilities, A & C
  if (BC_c.gen == AC_c.gen) return;
  for_pos_cluster(AC_c, AC, ({
    if (is_pos_equal(AC, AB)) continue;
    if (sudoku->arr[AC.i][AC.j]) continue;
    if (poss_size(sudoku->possibilities[AC.i][AC.j]) != 2) continue;

    // ABC must match, though in different order
    val_t B, A, C;
    if (!_ABC(sudoku->possibilities[AB.i][AB.j], sudoku->possibilities[AC.i][AC.j],
      &B, &A, &C)) continue;
    if (!(ABC_A == A && ABC_B == B && ABC_C == C)) continue;

    _c_rm_from_BC(sudoku, AB, BC, AC, cluster(BC, col_c), A, B, C);
    _c_rm_from_BC(sudoku, AB, BC, AC, cluster(BC, row_c), A, B, C);
    _c_rm_from_BC(sudoku, AB, BC, AC, cluster(BC, box_c), A, B, C);
  }))
}

static void _bc_cluster(sudoku_t *sudoku, pos_t AB, cluster_t BC_c) {
  // find a BC that AB can see, and contains only two possibilities, B & C
  for_pos_cluster(BC_c, BC, ({
    if (is_pos_equal(BC, AB)) continue;
    if (sudoku->arr[BC.i][BC.j]) continue;
    if (poss_size(sudoku->possibilities[BC.i][BC.j]) != 2) continue;

    val_t A, B, C;
    if (!_ABC(sudoku->possibilities[AB.i][AB.j], sudoku->possibilities[BC.i][BC.j],
      &A, &B, &C)) continue;

    // redundant, col_c is scaned by BC
    // _ac_cluster(sudoku, AB, BC_c, BC, cluster(AB, col_c), A, B, C);
    if (BC_c.gen == &col_c) _ac_cluster(sudoku, AB, BC_c, BC, cluster(AB, row_c), A, B, C);
    _ac_cluster(sudoku, AB, BC_c, BC, cluster(AB, box_c), A, B, C);
  }))
}

STRATEGY("Y-Wing", 8) {
  // find AB, containing two possibilities, A & B
  for_pos_cluster_zero(all_c, AB, ({
    if (sudoku->arr[AB.i][AB.j]) continue;
    if (poss_size(sudoku->possibilities[AB.i][AB.j]) != 2) continue;
    _bc_cluster(sudoku, AB, cluster(AB, col_c));
    _bc_cluster(sudoku, AB, cluster(AB, row_c));
    // redundant, both scanned box_c for AC
    // _bc_cluster(sudoku, AB, cluster(AB, box_c));
  }));
  return stack_size(sudoku->decr_poss);
}
