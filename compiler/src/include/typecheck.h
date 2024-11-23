#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "expr.h"
#include "stmt.h"
#include "type.h"
#include "decl.h"

// helpers
struct type *type_copy(struct type *t);
int type_equals(struct type *a, struct type *b);
void type_delete(struct type *t);

// typecheckers
struct type *expr_typecheck(struct expr *e);
void stmt_typecheck(struct stmt *s);
void decl_typecheck(struct decl *d);

#endif 
