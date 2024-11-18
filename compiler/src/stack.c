#include <stdlib.h>
#include "../include/stack.h"
#include "../include/hash_table.h"
#define STACK_SIZE 3

stack_t* SYMBOL_STACK;

stack_t* init_stack() {
  stack_t* stack  = (stack_t*)malloc(sizeof(stack_t));
  stack->size=0;
  stack->data=(struct hash_table*)malloc(sizeof(struct hash_table) * STACK_SIZE);
  stack->sp = stack->data; 
  return stack;
}

void stack_push(struct hash_table table) {
  int block_size = sizeof(struct hash_table);
  memcpy(SYMBOL_STACK->sp, table, block_size);

  // shift the stack top
  SYMBOL_STACK->sp = SYMBOL_STACK->sp + block_size;

  // resize
  ++SYMBOL_STACK->size;
}

void stack_pop() {  
  hash_table_clear(SYMBOL_STACK->sp);

  // shift the stack top
  int block_size = sizeof(struct hash_table);
  SYMBOL_STACK->sp = SYMBOL_STACK->sp - block_size;

  // resize
  --SYMBOL_STACK->size;
}

int stack_size() {
  return SYMBOL_STACK->size;
}

struct hash_table* get_stack_top() {
  return SYMBOL_STACK->sp;
}