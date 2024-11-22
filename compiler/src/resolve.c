#include "include/resolve.h"
#include "include/scope.h"
#include "include/constants.h"
#include "include/stack.h"

extern stack_t SYMBOL_STACK;

void decl_resolve(struct decl *d) {
  printf("DECL_RESOLVE: %p\n", d);
	if(!d) return;
	symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	d->symbol = symbol_create(kind,d->type,d->name);
	expr_resolve(d->value);
	scope_bind(d->name,d->symbol);
	if(d->code) {
    printf("I entered\n");
		scope_enter();
		param_list_resolve(d->type->params);
		stmt_resolve(d->code);
    stack_print(&SYMBOL_STACK);
		scope_exit();
	}
	decl_resolve(d->next);
}

// for every statement, create a scope.. with a body / block statement
void stmt_resolve(struct stmt *s) {
  printf("STMT_RESOLVE: %p\n", s);
	if (!s) return;
	switch (s->kind) {
		case STMT_BLOCK:
      printf("STMT_BLOCK\n");
      stmt_resolve(s->body);
			break;
		case STMT_DECL:
      printf("STMT_DECL\n");
      decl_resolve(s->decl);
			break;
    case STMT_EXPR:
      printf("STMT_EXPR\n");
      break;
    case STMT_IF:
      printf("STMT_IF\n");
      break;
    case STMT_IF_ELSE:
      printf("STMT_IF_ELSE\n");
      break;
    case STMT_FOR:
      printf("STMT_FOR\n");
      break;
    case STMT_PRINT:
      printf("STMT_PRINT\n");
      break;
    case STMT_RETURN:
      printf("STMT_RETURN\n");
      break;
	}
  stmt_resolve(s->next); // if its a statement_list
}

void expr_resolve(struct expr *e) {
  printf("EXPR_RESOLVE: %p\n", e);
  if(!e) return;
  if(e->kind==EXPR_NAME) {
    printf("EXPR_RESOLVE_EXPR_NAME\n");
    e->symbol = scope_lookup(e->name);
    if(e->symbol->kind == SYMBOL_GLOBAL) {
        printf("=======> %s resolves to %s %s <=======\n", e->symbol->name, SCOPE_LOOKUP[e->symbol->kind], e->symbol->name);
    } else {
        printf("=======> %s resolves to %s %d <=======\n", e->symbol->name, SCOPE_LOOKUP[e->symbol->kind], e->symbol->which);
    }
  } else {
    expr_resolve( e->left );
    expr_resolve( e->right );
  }
}

void param_list_resolve(struct param_list *p) {
    printf("PARAM_LIST_RESOLVE\n");
  	if(!p) {
    	return;
  	}
    // create a symbol for hashtable
    p->symbol = symbol_create(SYMBOL_PARAM, p->type,p->name);
    scope_bind(p->name, p->symbol);

    param_list_resolve(p->next);
}
