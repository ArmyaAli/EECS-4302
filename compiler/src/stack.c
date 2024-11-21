#include <stdlib.h>
#include <stdio.h>
#include "include/stack.h"
#include "include/hash_table.h"
#include "include/constants.h"
#include "include/symbol.h"

void stack_init(stack_t *stack) {
    stack->data = (struct hash_table*)malloc(sizeof(struct hash_table) * STACK_SIZE);
    stack->size = 0;
    stack->top = 0;
    stack->MAX_SIZE = STACK_SIZE;
    // create a default global scope
    stack_push(stack, *hash_table_create(1, 0));
}

// TODO(Ali): realloc data if approaching STACK_SIZE;
void stack_push(stack_t *stack, struct hash_table table) {
  // add to the end
  if(stack->size > 0) stack->top++;
  stack->data[stack->top] = table;
  ++stack->size;
}

int stack_empty(stack_t *stack) {
    return stack->size == 0;
}

struct hash_table stack_pop(stack_t *stack) {
  //  printf("stack pointer: %d\n", stack->top);
    struct hash_table* entry = &stack->data[stack->top];
  //  printf("entry: %p\n", &entry);
    struct hash_table temp = *entry;
    // create a copy
    if(stack->top > 0) { 
      --stack->top;
      --stack->size;

      char *key = NULL;
      struct symbol* sym = NULL;
      hash_table_firstkey(entry);
      int next = hash_table_nextkey(entry, &key, (void*)&sym);
      while(next) {
        if(sym != NULL) symbol_destroy(sym);
        next = hash_table_nextkey(entry, &key, (void*)&sym);
      }
      //hash_table_delete(entry);
    } else {
      stack->top = 0;
      stack->size = 0;
      stack->data = NULL;
    }

    return temp;
}

struct hash_table* stack_peek(stack_t *stack) {
//    printf("in stack_peek %p\n", stack);
 //   printf("%d\n", stack->top);
    return &stack->data[stack->top];
}

int stack_size(stack_t *stack) {
    return stack->size;
}

void stack_destroy(stack_t *stack) {
    //for(int i = 0; i < stack->size; ++i) {
    //  struct hash_table* entry = &stack->data[i];
    //  hash_table_delete(entry);
    //}
}

void stack_print(stack_t *stack) {
  // for each entry in the array
  printf("BEGIN PRINT STACK\n");
  for(int i = 0; i < stack->size; ++i) {
    // get the hash table
    struct hash_table* table = &stack->data[i];
    printf("\tHASH_TABLE index: %d, value: %p\n", i, table);
    // Iterate through each bucket in the hash_table
    for (int i = 0; i < table->bucket_count; i++) {
      struct entry *current_entry = table->buckets[i];
      while (current_entry != NULL) {
        printf("\t\tKey: %s, Value: %s\n", 
          current_entry->key, 
          TYPE_LOOKUP[((struct symbol*)current_entry->value)->type->kind]
        );
        current_entry = current_entry->next;
      }
    }
  }
  printf("END PRINT STACK\n");
}
