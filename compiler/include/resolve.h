#ifndef RESOLVE_H
#define RESOLVE_H

#include "decl.h"
#include "stmt.h"
#include "expr.h"
#include "param_list.h"

void decl_resolve(struct decl *d);
void stmt_resolve(struct stmt *s);
void expr_resolve(struct expr *e);
void param_list_resolve(struct param_list *p);

#endif
