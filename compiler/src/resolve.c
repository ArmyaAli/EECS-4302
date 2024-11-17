#include "../include/resolve.h"
#include "../include/scope.h"
#include "../include/print.h"

void decl_resolve(struct decl *d) {
	if(!d) return;
	symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	d->symbol = symbol_create(kind,d->type,d->name);
	printf("literal_value: %d\n", d->value->literal_value);
	printf("kind: %p\n", (*(d->value)).left);
	expr_resolve(d->value);
	scope_bind(d->name,d->symbol);
	if(d->code) {
		scope_enter();
		param_list_resolve(d->type->params);
		stmt_resolve(d->code);
		scope_exit();
	}
	decl_resolve(d->next);
}

void stmt_resolve(struct stmt *s) {
	switch (s->kind)
	{
	case STMT_EXPR:
		stmt_print(s, 0, 0);
		// expr_resolve(s->expr);
		break;
	
	default:
		break;
	}
}

void expr_resolve(struct expr *e) {
	printf("HAJSDK;FLKASDKL;JFASL;KDJ");
  if(!e)return;
  if(e->kind==EXPR_NAME) {
	printf("exp\n");
    e->symbol = scope_lookup(e->name);
  } else {
	printf("aslkjs;daf %p", e->left);
    expr_resolve(e->left);
    expr_resolve(e->right);
  }
}

void param_list_resolve(struct param_list *p) {


}
