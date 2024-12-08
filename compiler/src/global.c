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
