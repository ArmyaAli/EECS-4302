#ifndef STACK_H
#define STACK_H
#include "hash_table.h"
#define STACK_SIZE 512

// Define the stack node for the hash_table
typedef struct stack_node {
    struct hash_table data;
    struct stack_node *next;
} stack_node;

// Define the stack structure
typedef struct stack {
    int sp;
    struct hash_table *data;
    int size;
    int MAX_SIZE;
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
