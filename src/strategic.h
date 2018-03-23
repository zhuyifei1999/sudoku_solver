#ifndef STRATEGIC_H_
#define STRATEGIC_H_

#include "compat.h"
#include "sudoku.h"
#include "stack.h"

typedef struct pos_val_t {
  pos_t pos;
  val_t val;
} pos_val_t;

typedef struct sudoku_t {
  val_t arr[9][9];
  val_t *possibilities[9][9];
  struct_stack(pos_t, ns_pos, 9*9)
  struct_stack(pos_val_t, decr_poss, 9*9*9)
} sudoku_t;

// remove the val possibility of sudoku at position
bool decr_possible(sudoku_t *sudoku, pos_t position, val_t val);

// remove the possibilities from position; if intersect is true, keep only intersections
bool truncate_possible(sudoku_t *sudoku, pos_t position, val_t *poss_arr, bool intersect);

// determine the value of sudoku at position to be val
void place(sudoku_t *sudoku, pos_t position, val_t val);

void solve_sudoku(sudoku_arr sudoku);

typedef struct strategy_t {
  char *name;
  bool (*func)(sudoku_t *);
  unsigned short order;
} strategy_t;

struct all_strategies_t {
  bool initialized;
  strategy_t arr[100];
};

extern struct all_strategies_t all_strategies;

#define STRATEGY(name_val, order_val) \
  static bool __strategy(sudoku_t *sudoku); \
  INITIALIZER(__strategy_init) { \
    size_t i; \
    for (i = 0; (all_strategies.arr[i].func); i++) {} \
    all_strategies.arr[i] = (strategy_t){ \
      .name = name_val, \
      .func = &__strategy, \
      .order = order_val \
    }; \
    debug_print("Registered strategy %s with order %hd at index %zd", name_val, order_val, i); \
  } \
  static bool __strategy(sudoku_t *sudoku)


#endif
