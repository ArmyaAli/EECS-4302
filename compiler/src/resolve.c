#include "../include/resolve.h"
#include "../include/scope.h"
#include "../include/constants.h"
#include "../include/stack.h"

extern stack_t SYMBOL_STACK;

void decl_resolve(struct decl *d) {
	if(!d) return;
	symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
	d->symbol = symbol_create(kind,d->type,d->name);
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
	decl_resolve(d->next);
}

// for every statement, create a scope.. with a body / block statement
void stmt_resolve(struct stmt *s) {
	if (!s) return;
	switch (s->kind) {
		case STMT_BLOCK:
      printf("BLOCK\n");
      scope_enter();
			stmt_resolve(s->body);
      scope_exit();
			break;
		case STMT_DECL:
      printf("DECL\n");
			decl_resolve(s->decl);
			stmt_resolve(s->next); // resolve further if we have more than 1 stmts
			break;
    case STMT_EXPR:
      printf("EXPR\n");
      expr_resolve(s->expr);
      break;
    case STMT_IF:
      printf("STMT_IF\n");
      stmt_resolve(s->body);
      break;
    case STMT_IF_ELSE:
      printf("STMT_IF_ELSE\n");
      stmt_resolve(s->body);
      stmt_resolve(s->else_body);
      break;
    case STMT_FOR:
      printf("STMT_FOR\n");
      stmt_resolve(s->body);
      break;
    case STMT_PRINT:
      printf("STMT_PRINT\n");
      printf("left: %s\n", s->expr->left->name);
      printf("right: %s\n", s->expr->right->name);
      expr_resolve(s->expr);
      // do nothing
      break;
    case STMT_RETURN:
      printf("STMT_RETURN\n");
      expr_resolve(s->expr);
      scope_exit();
      break;
	}
}

void expr_resolve(struct expr *e) {
  if(!e) {
    return;
  }
  if(e->kind==EXPR_NAME) {
    e->symbol = scope_lookup(e->name);
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
