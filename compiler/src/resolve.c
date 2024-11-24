#include "include/resolve.h"
#include "include/scope.h"
#include "include/constants.h"

int current_stmt_type = -1;

void decl_resolve(struct decl *d) {
	if(!d) return;
    symbol_t kind = scope_level() > 1 ? SYMBOL_LOCAL : SYMBOL_GLOBAL;
    d->symbol = symbol_create(kind,d->type,d->name);


    if(d->value) {
      symbol_create_helper(&d->value);
      if(d->value->kind == EXPR_CALL) {
        expr_resolve(d->value->left);
      } else {
        expr_resolve(d->value);
      }
    }

   scope_bind(d->name,d->symbol);
   stack_print(&SYMBOL_STACK);
	if(d->code) {
    current_stmt_type = FUNC;
		scope_enter();
		param_list_resolve(d->type->params);
    current_stmt_type = -1;
		stmt_resolve(d->code);

    // If we have a return statement, we should popoff stack till global scope 
    // or exit scope as normal
    if(current_stmt_type == RETURN_TYPE && 
      stack_size(&SYMBOL_STACK) > 1) {
      return;
    }
	}
	decl_resolve(d->next);
}

// for every statement, create a scope.. with a body / block statement
void stmt_resolve(struct stmt *s) {
	if (!s) return;
	switch (s->kind) {
		case STMT_BLOCK:
      switch(current_stmt_type) {
        case FUNC:
          printf("IN FUNC\n");
          stmt_resolve(s->body);
          scope_exit();
          break;
        case FOR:
          stmt_resolve(s->body);
          scope_exit();
          break;
        default:
          scope_enter();
          stmt_resolve(s->body);
          scope_exit();
      }
			break;
		case STMT_DECL:
      decl_resolve(s->decl);
			break;
    case STMT_EXPR:
      struct expr* e = s->expr;

      struct symbol* symb = NULL;
      switch (e->kind) {
      case EXPR_NAME:
        symb = scope_lookup(e->name);
        e->symbol = symbol_copy(symb); // deep copy is needed since we have recurive struct types
        break;
      case EXPR_INCR:
      case EXPR_DECR:
        symb = scope_lookup(e->left->name);
        e->left->symbol = symbol_copy(symb); // deep copy is needed since we have recurive struct types
        break;
      default:
        break;
      }
      expr_resolve(s->expr);
      break;
    case STMT_IF_ELSE:
      //FUNC = 0;
      current_stmt_type = -1;
      if (!s->expr) expr_resolve(s->expr);
      stmt_resolve(s->body);
      stmt_resolve(s->else_body);
      break;
    case STMT_IF:
      current_stmt_type = -1;
      expr_resolve(s->expr);
      stmt_resolve(s->body);
      break;
    case STMT_FOR:
      current_stmt_type = FOR;
      struct expr* init_expr = s->init_expr->left;
      struct expr* init_expr_value = s->init_expr->right;
      struct symbol* symbol = symbol_create(SYMBOL_LOCAL, type_create(TYPE_INTEGER, NULL, NULL), init_expr->name);
      scope_enter();
      scope_bind(init_expr->name, symbol);
      expr_resolve(init_expr_value);
      expr_resolve(s->expr);
      expr_resolve(s->next_expr);
      stmt_resolve(s->body);
      break;
    case STMT_PRINT:
      expr_resolve(s->expr);
      break;
    case STMT_RETURN:
      current_stmt_type = RETURN_TYPE;
      if(s->expr) expr_resolve(s->expr);
      scope_exit();
      printf("IN RETURN\n");
      return;
	}
  stmt_resolve(s->next); // if its a statement_list
}

void expr_resolve(struct expr *e) {
  if(!e) return;
  if(e->kind==EXPR_NAME) {
    e->symbol = scope_lookup(e->name);
    if(e->symbol && e->symbol->kind == SYMBOL_GLOBAL) {
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
  	if(!p) {
    	return;
  	}
    // create a symbol for hashtable
    p->symbol = symbol_create(SYMBOL_PARAM, p->type,p->name);
    scope_bind(p->name, p->symbol);

    param_list_resolve(p->next);
}

void symbol_create_helper(struct expr** e) {
    struct expr* exp = *e;
    struct symbol* s = NULL;
    if(!exp) return;

    switch(exp->kind) {
      case EXPR_NAME:
        s = scope_lookup(exp->name);
        exp->symbol = symbol_copy(s); // deep copy is needed since we have recurive struct types
        break;
      case EXPR_CALL:
        s = scope_lookup(exp->left->name);
        (*e)->left->symbol = symbol_copy(s); // deep copy is needed since we have recurive struct types
        break;
      default:
    }
}
