#include <stdlib.h>
#include <stdio.h>
#include "../include/stack.h"
#include "../include/hash_table.h"
#include "../include/print.h"
#define STACK_SIZE 512

stack_t SYMBOL_STACK;

// Function to initialize the stack
void stack_init(stack_t *stack) {
    stack->top = NULL;
    stack->size = 0;
    // create a default global scope
    struct hash_table* table = hash_table_create(1, 0);
    stack_push(&SYMBOL_STACK, *table);
}

// Function to push a hash_table onto the stack
void stack_push(stack_t *stack, struct hash_table table) {
    stack_node *new_node = (stack_node *)malloc(sizeof(stack_node));
    new_node->data = table;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;
}

// Function to check if the stack is empty
int stack_empty(stack_t *stack) {
    return stack->size == 0;
}

// Function to pop a hash_table from the stack
struct hash_table stack_pop(stack_t *stack) {
    stack_node *temp = stack->top;
    struct hash_table table = temp->data;
    stack->top = stack->top->next;
    free(temp);
    stack->size--;
    return table;
}

// Function to peek at the top hash_table on the stack without removing it
struct hash_table stack_peek(stack_t *stack) {
    return stack->top->data;
}

// Function to get the size of the stack
int stack_size(stack_t *stack) {
    return stack->size;
}

// Function to free the stack
void stack_destroy(stack_t *stack) {
    while (!stack_empty(stack)) {
        stack_pop(stack);
    }
}

// Function to print the contents of the stack
void stack_print(stack_t *stack) {
	 stack_node *current = stack->top;
    int table_index = 0;

    // Iterate through each hash_table in the stack
    while (current != NULL) {
        struct hash_table *table = &current->data;
        printf("Hash Table %d:\n", table_index);
        printf("  Bucket Count: %d\n", table->bucket_count);
        printf("  Size: %d\n", table->size);

        // Iterate through each bucket in the hash_table
        for (int i = 0; i < table->bucket_count; i++) {
            struct entry *current_entry = table->buckets[i];

            // Print each entry in the bucket
            printf("  Bucket %d:\n", i);
            while (current_entry != NULL) {
                printf("    Key: %s, Value: %d\n", current_entry->key, ((struct symbol*)current_entry->value)->type->kind);
                current_entry = current_entry->next;
            }
        }

        // Move to the next hash_table in the stack
        current = current->next;
        table_index++;
    }
    //symbol_print((struct symbol*)(current->data.ientry->value));
}
