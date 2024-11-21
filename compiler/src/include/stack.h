#ifndef STACK_H
#define STACK_H
#include "hash_table.h"
#define STACK_SIZE 512

// Define the stack structure
typedef struct stack {
    int top;
    int size;
    int MAX_SIZE;
    struct hash_table *data;
} stack_t;

void stack_init(stack_t* stack);
void stack_destroy(stack_t *stack);
void stack_push(stack_t *stack, struct hash_table table);
int stack_empty(stack_t *stack);
int stack_size(stack_t *stack);
void stack_print(stack_t *stack);
struct hash_table stack_pop(stack_t *stack); 
struct hash_table* stack_peek(stack_t *stack);

#endif 
