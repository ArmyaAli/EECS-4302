#include "include/codegen.h"
#include <stdlib.h>

extern char* asm_output;
// Allocate 1 MegaByte to assembler output file by default
void init_asm_output() {
  asm_output = (char*)malloc(sizeof(char) * 1024 * 1000);
}

void destroy_asm_output() {
  free(asm_output);
}
