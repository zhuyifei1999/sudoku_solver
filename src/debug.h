#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#define debug_print(fmt, ...) do { \
  fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
          __LINE__, __func__, __VA_ARGS__); \
} while (0)
#else
#define debug_print(...) do {} while (0)
#endif

#endif
