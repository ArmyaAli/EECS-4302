#include "include/symbol.h"
#include <stdlib.h>
#include <typecheck.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {
	struct symbol *s = malloc(sizeof(*s));
	s->kind = kind;
	s->type = type;
	s->name = name;
	s->which = 0;
  return s;
}

void symbol_destroy(struct symbol* sym) {
  free(sym);
}

struct symbol* symbol_copy(struct symbol* s) {
    if (!s) return NULL;

    struct symbol *new_symbol = (struct symbol*) malloc(sizeof(struct symbol));
    if (!new_symbol) return NULL;

    new_symbol->kind = s->kind;
    new_symbol->which = s->which;

    // Deep copy the type
    new_symbol->type = (struct type*) malloc(sizeof(struct type));
    new_symbol->type = s->type ? type_copy(s->type) : NULL;

    // Deep copy the name
    new_symbol->name = s->name ? strdup(s->name) : NULL;

    return new_symbol;
}
