#include "../include/resolve.h"
#include "../include/scope.h"
#include "../include/constants.h"

void decl_resolve(struct decl *d) {
	if(!d) return;
	symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	d->symbol = symbol_create(kind,d->type,d->name);
	printf("literal_value: %d\n", d->value->literal_value);
	expr_resolve(d->value);
	scope_bind(d->name,d->symbol);
	if(d->code) {
    printf("d->code\n");
		scope_enter();
		param_list_resolve(d->type->params);
		stmt_resolve(d->code);
		scope_exit();
	}
  printf("==================================================\n");
	decl_resolve(d->next);
}

// for every statement, create a scope.. with a body / block statement
void stmt_resolve(struct stmt *s) {
//	switch (s->kind) {
//	case STMT_EXPR:
//		stmt_print(s, 0, 0);
//		expr_resolve(s->expr);
//		break;
//	default:
//		break;
//	}
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


}
