#ifndef RESOLVE_H
#define RESOLVE_H

#include "decl.h"
#include "stmt.h"
#include "expr.h"
#include "stack.h"
#include "param_list.h"

typedef enum curr_stmt_type {
  FUNC,
  FOR,
  RETURN_TYPE
} curr_stmt_t;

void decl_resolve(struct decl *d);
void stmt_resolve(struct stmt *s);
void expr_resolve(struct expr *e);
void param_list_resolve(struct param_list *p);
void symbol_create_helper(struct expr** e);

extern stack_t SYMBOL_STACK;

#endif
