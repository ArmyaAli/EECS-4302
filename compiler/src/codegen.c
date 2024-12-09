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

const char* helper_remove_nl(char* str) {
  int found = -1;
  int i = 0;
  for(i = 0; i < strlen(str); ++i) {
    if(str[i] == '\n') {
      found = i;
      break;
    }
  }

  for(i = found; i < strlen(str); ++i) 
    str[i] = str[i+1];

  return str;
}
