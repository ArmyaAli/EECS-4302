#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "param_list.h"
#include "expr.h"

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );

#endif
