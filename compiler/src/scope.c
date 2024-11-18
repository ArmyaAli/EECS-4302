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
  struct hash_table* table = hash_table_create(0, 0);
  stack_push(table);
}

void scope_exit() {
  stack_pop();
}

int scope_level() {
  return stack_size();
}

void scope_bind(const char *name, struct symbol *sym) {
  struct hash_table* top = get_stack_top();

  // allocate memory for hashtable
  top->ientry->key = (char*)malloc(sizeof(name) * strlen(name));
  top->ientry->value = (struct symbol*)malloc(sizeof(struct symbol));

  int success = hash_table_insert(top, name, sym);
  if (!success) {
    printf("Error");
  }
}

struct symbol *scope_lookup(const char *name) {
  struct symbol* s = NULL;
  struct hash_table* top = get_stack_top();

  struct hash_table* current = top;
  while (current != NULL) {
    s = hash_table_lookup(current, name);

    // if variable name matched the one in the stack
    if (strcpy(s->name, name) == 0) {
      return s;
    }
    else {
      // go to the next one
    }
  }  
  return NULL; // no match found
}

struct symbol *scope_lookup_current(const char *name) {
  return NULL;

}

