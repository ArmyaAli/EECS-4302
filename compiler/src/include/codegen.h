#ifndef CODEGEN_H
#define CODEGEN_H

#include "expr.h"
#include "stmt.h"
#include "decl.h"
#include "symbol.h"
#include "hash_table.h"

typedef struct frame {
  char reg[5];
  char var_name[10];
  int offset;
} frame_t;

void first_pass(struct decl *d);
void expr_codegen_first_pass(struct expr* e);
void stmt_codegen_first_pass(struct stmt *s);

void second_pass(struct decl *d);
void expr_codegen_second_pass(struct expr* e);
void stmt_codegen_second_pass(struct stmt *s);


void init_asm_output();
void destroy_asm_output();

#endif
