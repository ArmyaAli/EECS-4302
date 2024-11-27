#include <register_helper.h>
#include <stdio.h>
#include <string.h>

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