#ifndef STACK_H_
#define STACK_H_

#define struct_stack(type, name, size) type name##_stack[size]; size_t name##_stacksize;

#define stack_init(name) name##_stacksize = 0
#define stack_size(name) name##_stacksize

#define stack_push(name, val) name##_stack[name##_stacksize++] = val
#define stack_pop(name) name##_stack[--name##_stacksize]

#endif
