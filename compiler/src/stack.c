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
