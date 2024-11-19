#include "../include/resolve.h"
#include "../include/scope.h"
#include "../include/constants.h"
#include "../include/stack.h"

extern stack_t SYMBOL_STACK;

void decl_resolve(struct decl *d) {
	if(!d) return;
	symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	d->symbol = symbol_create(kind,d->type,d->name);
	printf("%d\n", d->type->kind);
	// printf("literal_value: %d\n", d->value->literal_value);

	// create a local scope for function
	expr_resolve(d->value);
	scope_bind(d->name,d->symbol);
	if(d->code) {
		scope_enter();
		
		param_list_resolve(d->type->params);
		stack_print(&SYMBOL_STACK);

		stmt_resolve(d->code);
		scope_exit();
	}
  printf("==================================================\n");
	decl_resolve(d->next);
}

// for every statement, create a scope.. with a body / block statement
void stmt_resolve(struct stmt *s) {
	if (!s) return;
	printf("HEREEEEE: %d\n", s->kind);
	switch (s->kind) {
		case STMT_BLOCK:
			stmt_resolve(s->body);
			break;
		case STMT_DECL:
			decl_resolve(s->decl);
			break;
	}
}

void expr_resolve(struct expr *e) {
  if(!e) {
    return;
  }
  if(e->kind==EXPR_NAME) {
    e->symbol = scope_lookup(e->name);
    printf("%p\n", e->symbol);
    if(e->symbol->kind == SYMBOL_GLOBAL) {
        printf("%s resolves %s %s\n", e->symbol->name, SCOPE_LOOKUP[e->symbol->kind], e->symbol->name);
    } else {
        printf("%s resolves %s %d\n", e->symbol->name, SCOPE_LOOKUP[e->symbol->kind], e->symbol->which);
    }
  } else {
    expr_resolve(e->left);
    expr_resolve(e->right);
  }
}

void param_list_resolve(struct param_list *p) {
  	if(!p) {
    	return;
  	}

	// create a symbol for hashtable
	p->symbol = symbol_create(SYMBOL_LOCAL,p->type,p->name);
	scope_bind(p->name,p->symbol);

	param_list_resolve(p->next);
}