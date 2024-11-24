#include "include/resolve.h"
#include "include/scope.h"
#include "include/constants.h"

int current_stmt_type = -1;

void decl_resolve(struct decl *d) {
  printf("DECL_RESOLVE: %p\n", d);
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
    printf("Function Detected \n");
    current_stmt_type = FUNC;
		scope_enter();
		param_list_resolve(d->type->params);
		stmt_resolve(d->code);

    // If we have a return statement, we should popoff stack till global scope 
    // or exit scope as normal
    if(current_stmt_type == RETURN_TYPE && 
        stack_size(&SYMBOL_STACK) > 1) return;
	}
	decl_resolve(d->next);
}

// for every statement, create a scope.. with a body / block statement
void stmt_resolve(struct stmt *s) {
  printf("STMT_RESOLVE: %p\n", s);
	if (!s) return;
	switch (s->kind) {
		case STMT_BLOCK:
      printf("STMT_BLOCK: %d\n", FUNC);
      switch(current_stmt_type) {
        case FUNC:
          printf("FUNC\n");
          stmt_resolve(s->body);
          scope_exit();
          break;
        case FOR:
          printf("FOR\n");
          stmt_resolve(s->body);
          scope_exit();
          break;
        default:
          printf("NON-FUNC-BLOCK\n");
          scope_enter();
          stmt_resolve(s->body);
          scope_exit();
      }
			break;
		case STMT_DECL:
      printf("STMT_DECL\n");
      decl_resolve(s->decl);
			break;
    case STMT_EXPR:
      printf("STMT_EXPR\n");
      struct expr* e = s->expr;
      printf("ekind %s\n", EXPR_LOOKUP[e->kind]);

      struct symbol* symb = NULL;
      switch (e->kind) {
      case EXPR_NAME:
        printf("SYMBOL CREATED for %s\n", e->name);
        symb = scope_lookup(e->name);
        printf("%p s->type: %symb\n",symb, TYPE_LOOKUP[symb->type->kind]);
        e->symbol = symbol_copy(symb); // deep copy is needed since we have recurive struct types
        break;
      case EXPR_INCR:
      case EXPR_DECR:
        printf("SYMBOL CREATED for %s\n", e->left->name);
        symb = scope_lookup(e->left->name);
        printf("%p s->type: %s\n",symb, TYPE_LOOKUP[symb->type->kind]);
        e->left->symbol = symbol_copy(symb); // deep copy is needed since we have recurive struct types
        break;
      default:
        printf("No expressions matched\n");
        break;
      }
      expr_resolve(s->expr);
      break;
    case STMT_IF_ELSE:
     // FUNC = 0;
      current_stmt_type = -1;
      printf("STMT_IF_ELSE\n");
      if (!s->expr) expr_resolve(s->expr);
      stmt_resolve(s->body);
      stmt_resolve(s->else_body);
      break;
    case STMT_IF:
      current_stmt_type = -1;
      printf("STMT_IF\n");
      expr_resolve(s->expr);
      stmt_resolve(s->body);
      break;
    case STMT_FOR:
      current_stmt_type = FOR;
      printf("STMT_FOR\n");
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
      printf("STMT_PRINT\n");
      expr_resolve(s->expr);
      break;
    case STMT_RETURN:
      current_stmt_type = RETURN_TYPE;
      printf("STMT_RETURN\n");
      if(s->expr) expr_resolve(s->expr);
      printf("anything\n");
      while(stack_size(&SYMBOL_STACK) > 1) scope_exit();
      return;
	}
  stmt_resolve(s->next); // if its a statement_list
}

void expr_resolve(struct expr *e) {
  printf("EXPR_RESOLVE: %p\n", e);
  if(!e) return;
  if(e->kind==EXPR_NAME) {
    printf("EXPR_RESOLVE_EXPR_NAME\n");
    e->symbol = scope_lookup(e->name);
    // struct type* t = type_create(e->symbol->kind)
//    printf("+++ %p %s -> %d -> %d -> %s\n",e, e->symbol->name, e->symbol->which, e->symbol->kind, TYPE_LOOKUP[e->symbol->type->kind]);
    if(e->symbol && e->symbol->kind == SYMBOL_GLOBAL) {
        printf("=======> %s resolves to %s %s <=======\n", e->symbol->name, SCOPE_LOOKUP[e->symbol->kind], e->symbol->name);
    } else {
        //printf("=======> %s resolves to %s %d <=======\n", e->symbol->name, SCOPE_LOOKUP[e->symbol->kind], e->symbol->which);
        printf("============== %p ==============\n", e->symbol);
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

void symbol_create_helper(struct expr** e) {
    struct expr* exp = *e;
    struct symbol* s = NULL;
//    if(!exp) return;

    switch(exp->kind) {
      case EXPR_NAME:
        printf("%s\n", exp->name);
        s = scope_lookup(exp->name);
        exp->symbol = symbol_copy(s); // deep copy is needed since we have recurive struct types
        break;
      case EXPR_CALL:
        s = scope_lookup(exp->left->name);
        printf("HELLO ALI: %s\n", exp->left->name);
        (*e)->left->symbol = symbol_copy(s); // deep copy is needed since we have recurive struct types
        break;
      default:
        printf("default case\n");
    }
}
