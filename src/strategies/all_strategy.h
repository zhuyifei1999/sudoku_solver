#ifndef ALL_STRATEGY_H_
#define ALL_STRATEGY_H_

#include "../strategic.h"

// A good video: https://www.youtube.com/watch?v=b123EURtu3I

bool naked_single(sudoku *sudoku);
bool hidden_single(sudoku *sudoku);
bool naked_pair_plus(sudoku *sudoku);
bool pointing_pair_plus(sudoku *sudoku);

#endif
