#ifndef CODEGEN_H
#define CODEGEN_H

#include "expr.h"
#include "stmt.h"
#include "decl.h"
#include "symbol.h"
#include "hash_table.h"

void expr_codegen(struct expr *e);
void stmt_codegen(struct stmt *s);
void decl_codegen(struct decl *d);
void expr_gen_first_pass(struct expr* e);
void first_pass(struct decl *d);
void second_pass(struct decl *d);

#endif
