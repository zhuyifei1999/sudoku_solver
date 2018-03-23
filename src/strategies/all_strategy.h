#ifndef ALL_STRATEGY_H_
#define ALL_STRATEGY_H_

#include "../strategic.h"

// With explanations from:
// * https://www.youtube.com/watch?v=b123EURtu3I
// * http://www.sudokuwiki.org/Strategy_Families

bool naked_single(sudoku_t *sudoku);
bool hidden_single(sudoku_t *sudoku);
bool naked_candidates(sudoku_t *sudoku);
bool intersection_removal(sudoku_t *sudoku);
bool hidden_candidates(sudoku_t *sudoku);
bool x_wing(sudoku_t *sudoku);
bool singles_chains(sudoku_t *sudoku);
bool y_wing(sudoku_t *sudoku);
bool swordfish(sudoku_t *sudoku);

bool (*strategies[])(sudoku_t *) = {
  &naked_single,
  &hidden_single,
  &naked_candidates,
  &intersection_removal,
  &hidden_candidates,
  &x_wing,
  &singles_chains,
  &y_wing,
  &swordfish,
  NULL
};

#endif
