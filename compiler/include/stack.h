#ifndef STACK_H
#define STACK_H

typedef struct stack {
  int size;
  struct hash_table* sp; // stackpointer
  struct hash_table* data;
} stack_t;

stack_t* init_stack();

#endif 
