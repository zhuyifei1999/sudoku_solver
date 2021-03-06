#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG

#ifdef DEBUG
#include <stdio.h>
#include <string.h>
// https://stackoverflow.com/a/8488201
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define debug_print(fmt, ...) do { \
  fprintf(stderr, "%s:%d: " fmt "\n", __FILENAME__, \
          __LINE__, __VA_ARGS__); \
} while (0)
#define debug_print_arr(buffer_main_size, buffer_temp_size, fmt, arr) do { \
  char buffer_main[(buffer_main_size)+1] = {0}; \
  char buffer_temp[(buffer_temp_size)+1] = {0}; \
  typeof(arr[0]) val; \
  for (int i = 0; (val = arr[i]); i++) { \
    if (i) strncat(buffer_main, ", ", buffer_main_size); \
    snprintf(buffer_temp, buffer_temp_size, fmt, val); \
    strncat(buffer_main, buffer_temp, buffer_main_size); \
  } \
  debug_print("[%s]", buffer_main); \
} while (0)
#else
#define debug_print(...) do {} while (0)
#define debug_print_arr(...) do {} while (0)
#endif

#define UNUSED(x) (void)(x)

#endif
