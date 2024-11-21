#include "include/scope.h"
#include "include/symbol.h"
#include "include/stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern stack_t SYMBOL_STACK;

void scope_enter() {
  struct hash_table* table = hash_table_create(1, 0);
  stack_push(&SYMBOL_STACK, *table);
}

void scope_exit() {
  stack_pop(&SYMBOL_STACK);
}

int scope_level() {
  return SYMBOL_STACK.size;
}

void scope_bind(const char *name, struct symbol *sym) {
  struct hash_table* top = stack_peek(&SYMBOL_STACK);
  printf("%p\n", top);
  sym->which = hash_table_size(top);
  hash_table_insert(top, name, sym);
  int s = hash_table_size(top);
  printf("after insersion: %d\n", s);
}

struct symbol *scope_lookup(const char *name) {
  for(int i = 0; i < SYMBOL_STACK.size; ++i) {
    struct hash_table entry = SYMBOL_STACK.data[i];
    struct symbol* found = (struct symbol*)hash_table_lookup(&entry, name);
    if(found != NULL) return found;
  }
  return NULL;
}

struct symbol *scope_lookup_current(const char *name) {
  return NULL;
}
