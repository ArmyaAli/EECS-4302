#include "../include/stack.h"
#include "../include/hash_table.h"
#include <stdlib.h>

stack_t* SYMBOL_STACK;
int STACK_SIZE=65536;
int BUCKET_SIZE=65536;

stack_t* init_stack() {
  stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
  stack->size=0;
  stack->data=(struct hash_table*)malloc(sizeof(struct hash_table) * STACK_SIZE);
  stack->sp = stack->data; 
  return stack;
}
