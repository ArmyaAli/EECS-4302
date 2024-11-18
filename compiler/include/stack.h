#ifndef STACK_H
#define STACK_H

typedef struct stack {
  int size;
  struct hash_table* sp; // stackpointer
  struct hash_table* data;
} stack_t;

stack_t* init_stack();
void stack_push();
void stack_pop();
int stack_size();
struct hash_table* get_stack_top();

#endif 
