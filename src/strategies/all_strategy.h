#ifndef ALL_STRATEGY_H_
#define ALL_STRATEGY_H_

#include "../strategic.h"

// With explanations from:
// * https://www.youtube.com/watch?v=b123EURtu3I
// * http://www.sudokuwiki.org/Strategy_Families

bool naked_single(sudoku *sudoku);
bool hidden_single(sudoku *sudoku);
bool naked_candidates(sudoku *sudoku);
bool intersection_removal(sudoku *sudoku);
bool hidden_candidates(sudoku *sudoku);

bool (*strategies[])(sudoku *) = {
  &naked_single,
  &hidden_single,
  &naked_candidates,
  &intersection_removal,
  &hidden_candidates,
  NULL
};

#endif
