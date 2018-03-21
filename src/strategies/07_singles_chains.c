#include "../strategic.h"
#include "../debug.h"
#include "../possibility.h"
#include "../stack.h"

#include <assert.h>
#include <stdlib.h>

typedef struct color_t {
  unsigned short id;
  bool color;
} color_t;

typedef struct _state {
  sudoku_t *sudoku;
  color_t tbl[9][9];
  val_t val;
  unsigned short id;
  struct_stack(color_t, collisions, 9*9)
} _state;

static void _color_recurse(_state *state, pos_t pos, bool color);

static void _color_pairs_gen(_state *state, pos_t position,
    cluster_gen_t gen, bool color) {
  pos_t *next = NULL;

#define do_return() do { if (next) free(next); return; } while (0)

  for_pos_cluster(cluster(position, gen), pos, ({
    if (is_pos_equal(pos, position)) continue;
    if (is_val_possible(state->sudoku->possibilities[pos.i][pos.j], state->val)) {
      if (next) do_return();
      next = malloc(sizeof(pos_t));
      memcpy(next, &pos, sizeof(pos_t));
    }
  }))

  _color_recurse(state, *next, color);
  do_return();
}

static void _color_recurse(_state *state, pos_t pos, bool color) {
  color_t old = state->tbl[pos.i][pos.j];
  if (old.id) {
    assert(old.id == state->id);
    if (old.color == color) return;
    // this loop collides with itself?!
    stack_push_check(state->collisions, old);
    stack_push_check(state->collisions, ((color_t){ .id = state->id, .color = color }));
    debug_print(printf_pos " %hd %d => %d: " printf_val "!!!", pos.i, pos.j,
      state->id, old.color, color, state->val);
  } else {
    // debug_print(printf_pos " %hd %d: " printf_val, pos.i, pos.j,
    //   state->id, color, state->val);
    state->tbl[pos.i][pos.j].id = state->id;
    state->tbl[pos.i][pos.j].color = color;
    _color_pairs_gen(state, pos, horz_c, !color);
    _color_pairs_gen(state, pos, vert_c, !color);
    _color_pairs_gen(state, pos, cell_c, !color);
  }
}

static void _cluster_color_see(_state *state, pos_t position,
    cluster_t cluster, unsigned short *seen, color_t this) {
  for_pos_cluster(cluster, pos, ({
    if (is_pos_equal(pos, position)) continue;
    color_t that = state->tbl[pos.i][pos.j];
    unsigned short *seen_color = seen + that.id;
    if (*seen_color & 2) {
      if ((*seen_color & 1) == that.color) continue;
      stack_push_check(state->collisions, this);
      debug_print(printf_pos ": " printf_val " sees both colors of id %hd",
        position.i, position.j, state->val, that.id);
    } else {
      *seen_color = 2 + that.color;
    }
    if (that.id == this.id && that.color == this.color) {
      stack_push_check(state->collisions, this);
      debug_print(printf_pos ": " printf_val " its own color of id %hd",
        position.i, position.j, state->val, that.id);
    }
  }))
}

bool singles_chains(sudoku_t *sudoku) {
  for_val(val) {
    _state state = { .sudoku = sudoku, .tbl = {{{0}}}, .val = val, .id = 0 };
    for_pos_cluster_zero(all_c, pos, ({
      if (sudoku->arr[pos.i][pos.j]) continue;
      if (state.tbl[pos.i][pos.j].id) continue;
      if (!is_val_possible(sudoku->possibilities[pos.i][pos.j], val)) continue;
      state.id++;
      _color_recurse(&state, pos, false);
    }))

    for_pos_cluster_zero(all_c, pos, ({
      if (!state.tbl[pos.i][pos.j].id) continue;
      color_t this = state.tbl[pos.i][pos.j];
      unsigned short seen[state.id+1];
      memset(seen, 0, (state.id+1)*sizeof(unsigned short));
      _cluster_color_see(&state, pos, cluster(pos, vert_c), seen, this);
      _cluster_color_see(&state, pos, cluster(pos, horz_c), seen, this);
      _cluster_color_see(&state, pos, cluster(pos, cell_c), seen, this);
    }))

    while (stack_size(state.collisions)) {
      color_t evil = stack_pop(state.collisions);

      for_pos_cluster_zero(all_c, pos, ({
        color_t this = state.tbl[pos.i][pos.j];
        if (this.id == evil.id && this.color == evil.color) {
          decr_possible(sudoku, pos, val);
        }
      }))
    }
  }

  return stack_size(sudoku->decr_poss);
}
