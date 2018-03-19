#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#include <string.h>
#define debug_print(fmt, ...) do { \
  fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
          __LINE__, __func__, __VA_ARGS__); \
} while (0)
#define debug_print_arr(buffer_main_size, buffer_temp_size, fmt, typ, arr) do { \
  char buffer_main[(buffer_main_size)+1] = {0}; \
  char buffer_temp[(buffer_temp_size)+1] = {0}; \
  typ val; \
  for (int i = 0; (val = arr[i]); i++) { \
    if (i) strncat(buffer_main, ", ", buffer_main_size); \
    snprintf(buffer_temp, buffer_temp_size, fmt, val); \
    strncat(buffer_main, buffer_temp, buffer_main_size); \
  } \
  debug_print("[%s]\n", buffer_main); \
} while (0)
#else
#define debug_print(...) do {} while (0)
#define debug_print_arr(...) do {} while (0)
#endif

#define UNUSED(x) (void)(x)

#endif
