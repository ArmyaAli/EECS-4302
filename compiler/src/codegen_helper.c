#include "include/symbol.h"
#include "include/expr.h"
#include "include/codegen_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/constants.h"

#define SCRATCH_REGISTERS 16

int label_counter = 0;

int scratch_registers[SCRATCH_REGISTERS];

// Allocate a scratch register
int scratch_alloc() {
    for (int i = 3; i < SCRATCH_REGISTERS; i++) {
        if (scratch_registers[i] == 0) { 
            scratch_registers[i] = 1; 
            return i;                   
        }
    }
    return -1;  
}

void scratch_free(int r) {
    if (r >= 0 && r < SCRATCH_REGISTERS) scratch_registers[r] = 0; 
}

// Get the name of a scratch register
const char* scratch_name(int r) {
    if (r >= 0 && r < SCRATCH_REGISTERS) return SCRATCH_LOOKUP[r];
    return NULL;  // Invalid register index
}

int label_create() {
    return label_counter++; // Increment and return the current counter value
}

char* label_name(int label) {
  char* labelstr = (char*)malloc(sizeof(char) * 32);
  sprintf(labelstr, ".L%d", label);
  return labelstr;
}

char *symbol_codegen(struct symbol* s) {
    char *result = NULL;
    int offset = 0;
    int byte_offset = 8;

    switch (s->kind) {
        case SYMBOL_GLOBAL:
            result = strdup(s->name);
            break;
        case SYMBOL_LOCAL:
        case SYMBOL_PARAM:
            offset = - (s->which * byte_offset) - byte_offset;
            result = (char *)malloc(32); // Enough space for "-123(%rbp)"
            sprintf(result, "%d(%%rbp)", offset);
            break;
        default:
            fprintf(stderr, "Error: Unknown symbol kind\n");
            exit(1);
    }
    return result;
}
