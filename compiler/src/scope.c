#include "../include/scope.h"
#include "../include/symbol.h"
#include "../include/stack.h"
#include <stdlib.h>
#include <string.h>

extern stack_t SYMBOL_STACK;

// push a new hash_table on the stack
// stack->data is the start of the stack memory
// stack->sp is the top of the stack
// we need to offset sp after adding something
// data = 0x00 + 64
// sp = 0x64 + 64
// 4 * 64 bytes = 256
void scope_enter() {
  struct hash_table* table = hash_table_create(1, 0);
  stack_push(&SYMBOL_STACK, *table);
  printf("****entered a new scope*****\n");
}

void scope_exit() {
  stack_pop(&SYMBOL_STACK);
}

int scope_level() {
  return SYMBOL_STACK.size;
}

void scope_bind(const char *name, struct symbol *sym) {
  struct hash_table top = stack_peek(&SYMBOL_STACK);
  printf("top pointer: %p\n", &top);
  int temp = hash_table_size(&top);
  printf("HASHTABLE_SIZE on ENTRY (%s): %d\n",name, temp);
  sym->which = temp;
  hash_table_insert(&top, name, sym);
  int as = hash_table_size(&top);
  printf("after insersion: %d\n", as);
}

struct symbol *scope_lookup(const char *name) {
  // stack_print(&SYMBOL_STACK);
  struct hash_table top = stack_peek(&SYMBOL_STACK);
  stack_node* cursor = SYMBOL_STACK.top;
  while(cursor != NULL) {
    struct symbol* found = (struct symbol *)hash_table_lookup(&top, name);
    if(found != 0) return found;
    cursor = cursor->next;
  }

  return NULL;
}

struct symbol *scope_lookup_current(const char *name) {
  return NULL;
}
