#include "include/global.h"

// GLOBALS
int EXIT_CODE = 0;

// AST
struct decl* parser_result;

// Data structures
stack_t SYMBOL_STACK;

int ERROR_COUNTER = 0;

// assembler output buffer
char* asm_output;
int asm_output_offset = 0;

frame_t stack[1000]; // will store variables that have registers
int sp = 0;
