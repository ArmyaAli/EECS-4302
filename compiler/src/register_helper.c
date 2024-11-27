#include <stdio.h>
#include <string.h>
#include <register_helper.h>
#include <symbol.h>

#define NUM_SCRATCH_REGISTERS 8

// Global counter for labels
static int label_counter = 0;

// Array to track the status of scratch registers (0 = free, 1 = allocated)
static int scratch_registers[NUM_SCRATCH_REGISTERS] = {0};

static const char* scratch_register_names[NUM_SCRATCH_REGISTERS] = {
    "rax", "rcx", "rdx", "r8", "r9", "r10", "r11", "r12"
};

// Allocate a scratch register
int scratch_alloc() {
    for (int i = 0; i < NUM_SCRATCH_REGISTERS; i++) {
        if (scratch_registers[i] == 0) { 
            scratch_registers[i] = 1; 
            return i;                   
        }
    }
    return -1;  // No free registers
}

// Free a previously allocated scratch register
void scratch_free(int r) {
    if (r >= 0 && r < NUM_SCRATCH_REGISTERS) {
        scratch_registers[r] = 0;  // Mark as free
    }
}

// Get the name of a scratch register
const char* scratch_name(int r) {
    if (r >= 0 && r < NUM_SCRATCH_REGISTERS) {
        return scratch_register_names[r];
    }
    return NULL;  // Invalid register index
}

// Create a new unique label
int label_create() {
    return label_counter++; // Increment and return the current counter value
}

// Get the string representation of a label
const char* label_name(int label) {
    static char buffer[32];
    snprintf(buffer, sizeof(buffer), ".L%d", label);
    return buffer;
}

const char *symbol_codegen(struct symbol *s) {
    char *result = NULL;

    switch (s->kind) {
        case SYMBOL_GLOBAL:
            result = strdup(s->name);
            break;

        case SYMBOL_PARAM: {
            int offset = -8 - (s->which * 8);
            result = (char *)malloc(20); // Enough space for "-123(%rbp)"
            snprintf(result, 20, "%d(%%rbp)", offset);
            break;
        }

        case SYMBOL_LOCAL: {
            int offset = -24 - (s->which * 8);
            result = (char *)malloc(20); // Enough space for "-123(%rbp)"
            snprintf(result, 20, "%d(%%rbp)", offset);
            break;
        }

        default:
            fprintf(stderr, "Error: Unknown symbol kind\n");
            exit(1);
    }

    return result;
}