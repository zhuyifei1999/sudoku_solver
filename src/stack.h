#ifndef STACK_H_
#define STACK_H_

#include <string.h>

#define struct_stack(type, name, size) type name##_stack[size]; size_t name##_stacksize;

// #define stack_init(name) name##_stacksize = 0
#define stack_size(name) name##_stacksize

#define stack_push(name, val) name##_stack[name##_stacksize++] = val
#define stack_pop(name) name##_stack[--name##_stacksize]

#define stack_push_check(name, val) do { \
  size_t __spc_i; \
  for (__spc_i = 0; __spc_i < name##_stacksize && \
    memcmp(name##_stack + __spc_i, &val, sizeof(typeof(val))) != 0; \
    __spc_i++) {} \
  if (__spc_i == name##_stacksize) stack_push(name, val); \
} while (0)

#endif
