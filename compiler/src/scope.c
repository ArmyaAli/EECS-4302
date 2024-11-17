#include "../include/scope.h"
#include "../include/symbol.h"
#include "../include/stack.h"
#include <stdlib.h>
#include <string.h>

extern stack_t* SYMBOL_STACK;

// push a new hash_table on the stack
// stack->data is the start of the stack memory
// stack->sp is the top of the stack
// we need to offset sp after adding something
// data = 0x00 + 64
// sp = 0x64 + 64
// 4 * 64 bytes = 256
//
void scope_enter() {
  int block_size = sizeof(struct hash_table);
  struct hash_table* table = hash_table_create(0, 0);
  memcpy(SYMBOL_STACK->sp, table, block_size);
  SYMBOL_STACK->sp = SYMBOL_STACK->sp + block_size;
  ++SYMBOL_STACK->size;
}

void scope_exit() {
  int block_size = sizeof(struct hash_table);
  free(SYMBOL_STACK->sp);
  SYMBOL_STACK->sp = SYMBOL_STACK->sp - block_size;
}

int scope_level() {
  return SYMBOL_STACK->size;
}

void scope_bind(const char *name, struct symbol *sym) {
  struct hash_table* top = SYMBOL_STACK->sp;

  top->ientry->key = (char*)malloc(sizeof(name) * strlen(name));
  top->ientry->value = (struct symbol*)malloc(sizeof(struct symbol));

  strcpy(top->ientry->key, name);
  memcpy(top->ientry->value, sym, sizeof(struct symbol));
}

struct symbol *scope_lookup(const char *name) {

}

struct symbol *scope_lookup_current(const char *name) {
  return NULL;

}

