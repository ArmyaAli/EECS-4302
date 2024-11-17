#include "../include/resolve.h"
#include "../include/scope.h"

void decl_resolve(struct decl *d) {
	if(!d) return;
	symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	d->symbol = symbol_create(kind,d->type,d->name);
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

}

void expr_resolve(struct expr *e) {
  if(!e) return;
  if(e->kind==EXPR_NAME) {
    e->symbol = scope_lookup(e->name);
  } else {
    expr_resolve( e->left );
    expr_resolve( e->right );
  }
}

void param_list_resolve(struct param_list *p) {


}
