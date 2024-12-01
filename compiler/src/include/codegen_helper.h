#ifndef CODEGEN_HELPER_H
#define CODEGEN_HELPER_H

#include "symbol.h"
#include "expr.h"

int scratch_alloc();
void scratch_free(int r);
const char* scratch_name(int r);

int label_create();
char* label_name(int label);

char *symbol_codegen(struct symbol *s);

#endif 
