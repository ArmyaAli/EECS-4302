#include <stdlib.h>
#include <stdio.h>
#include "../include/stack.h"
#include "../include/hash_table.h"
#include "../include/constants.h"

stack_t SYMBOL_STACK;

void stack_init(stack_t *stack) {
    stack->data = (struct hash_table*)malloc(sizeof(struct hash_table) * STACK_SIZE);
    stack->size = 0;
    // stack pointer is first index element in array at the start
    stack->sp = stack->size;
    stack->MAX_SIZE = STACK_SIZE;
    // create a default global scope
    struct hash_table* table = hash_table_create(1, 0);
    stack_push(&SYMBOL_STACK, *table);
}

// TODO(Ali): realloc data if approaching STACK_SIZE;
void stack_push(stack_t *stack, struct hash_table table) {
  // add to the end
  if(stack->size > 0) stack->sp++;
  stack->data[stack->sp] = table;
  ++stack->size;
}

int stack_empty(stack_t *stack) {
    return stack->size == 0;
}

struct hash_table stack_pop(stack_t *stack) {
    struct hash_table entry = stack->data[stack->sp];
    if(stack->sp > 0) { 
      --stack->sp;
      stack->size--;
    };

    return entry;
}

struct hash_table* stack_peek(stack_t *stack) {
    printf("in stack_peek %p\n", stack);
    printf("%d\n", stack->sp);
    return &stack->data[stack->sp];
}

int stack_size(stack_t *stack) {
    return stack->size;
}

void stack_destroy(stack_t *stack) {
    for(int i = 0; i < stack->sp; ++i) {
      //hash_table_delete(&stack->data[i]);
    }
    //free(stack);
}

void stack_print(stack_t *stack) {
  for(int i = 0; i < stack->size; ++i) {
    printf("entry: %p, table_size: %d, i: %d", &stack->data[i], stack->data[i].size, i);
  }
}
