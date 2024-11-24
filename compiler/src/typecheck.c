#include "include/typecheck.h"
#include "include/param_list.h"
#include "include/messages.h"
#include "include/constants.h"
#include "include/global.h"
#include <stdlib.h>
#include <string.h>

struct param_list *param_list_copy(struct param_list *params) {
    if (!params) return NULL;

    struct param_list *copy = param_list_create(params->name, params->type, params->next);
    copy->name = params->name ? strdup(params->name) : NULL;
    copy->type = type_copy(params->type);                   
    copy->symbol = params->symbol;                          
    copy->next = param_list_copy(params->next);             
    return copy;
}

// Function to copy a type structure
struct type *type_copy(struct type *t) {
    if (!t) return NULL;
    // Recursively copy the subtype and params
    struct type *subtype_copy = type_copy(t->subtype);
    struct param_list *params_copy = param_list_copy(t->params);
    // Create a new type and populate fields
    struct type *new_type = type_create(t->kind, subtype_copy, params_copy);
    new_type->array_size = t->array_size;
    return new_type;
}

void param_list_delete(struct param_list *params) {
    if (!params) return;
    param_list_delete(params->next);
    if (params->name) free(params->name);
    if (params->type) type_delete(params->type);
    free(params);
}

void type_delete(struct type *t) {
    if (!t) return;
    if (t->subtype) type_delete(t->subtype);
    if (t->params) param_list_delete(t->params);
    free(t);
}


int is_atomic(struct type *x) {
  switch (x->kind) {
  case TYPE_VOID:
  case TYPE_BOOLEAN:
  case TYPE_CHARACTER:
  case TYPE_INTEGER:
  case TYPE_STRING:
    return 1;
  default:
    return 0;
  }
}

int type_equals(struct type *a, struct type *b) {
  if (a->kind == b->kind) {
    if (is_atomic(a) && is_atomic(b)) {
      return 1;
    } else if (a->kind == TYPE_ARRAY && b->kind == TYPE_ARRAY) {
      return type_equals(a->subtype, b->subtype);
    } else if (a->kind == TYPE_FUNCTION && b->kind == TYPE_FUNCTION) {
      int func_type = type_equals(a->subtype, b->subtype);
      struct param_list* pl_a = a->params;
      struct param_list* pl_b = b->params;
      
      if(param_list_size(pl_a) != param_list_size(pl_b)) {
        printf(ERRORMSG_PARAM_LIST_SIZE);
        return 0;
      }
      struct param_list* cursor_a  = pl_a;
      struct param_list* cursor_b = pl_b;
      int res = -1;
      while(cursor_a != NULL) {
        if(cursor_a->type->kind != cursor_b->type->kind) res = 0;
        cursor_a = cursor_a->next;
        cursor_b = cursor_b->next;
      }
      if(res != 0) res = 1;
      return res && func_type;
    }
  }
  return 0;
}

// typecheckers
struct type *expr_typecheck(struct expr *e) {
  if (!e) return 0;
  struct type *lt = expr_typecheck(e->left);
  struct type *rt = expr_typecheck(e->right);
  struct type *result;

  if (e->kind == EXPR_DECR || e->kind == EXPR_INCR) {
    result = type_copy(e->symbol->type);
    return result;
  }

  switch (e->kind) {
  case EXPR_INTEGER_LITERAL:
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_STRING_LITERAL:
    result = type_create(TYPE_STRING, 0, 0);
    break;
  case EXPR_BOOLEAN_LITERAL:
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_CHAR_LITERAL:
    result = type_create(TYPE_CHARACTER, 0, 0);
    break;
  case EXPR_ADD:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf(ERRORMSG_TYPE_EXPR_ADD_ERROR, EXPR_LOOKUP[EXPR_ADD], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_SUB:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf(ERRORMSG_TYPE_EXPR_SUB_ERROR, EXPR_LOOKUP[EXPR_SUB], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_MUL:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf(ERRORMSG_TYPE_EXPR_MUL_ERROR, EXPR_LOOKUP[EXPR_MUL], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_DIV:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf(ERRORMSG_TYPE_EXPR_DIV_ERROR, EXPR_LOOKUP[EXPR_DIV], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_INCR:
    // if(lt->kind!=TYPE_INTEGER) {
    //   printf("\t <%s> can only be applied on an integer operator. ((%s) %s)\n",EXPR_LOOKUP[EXPR_INCR], TYPE_LOOKUP[lt->kind], e->left->name);
    //   ERROR_COUNTER ++;
    // }
    // result = type_create(TYPE_BOOLEAN, 0, 0);
    // NOTE: not working
    break;
  case EXPR_DECR:
    break;
  case EXPR_NAME:
    if (e->left && (e->left->kind == EXPR_DECR &&  e->left->kind == EXPR_INCR)) {
      result = type_copy(e->left->symbol->type);
    }
    else {
      if (e->kind == EXPR_NAME) {
          result = type_copy(e->symbol->type);
          break;
      } 
    }
  case EXPR_ASSIGN:
    if(!type_equals(lt, rt)) {
      printf("<< TYPE ERROR >>: Can not assign type of %s to %s\n", TYPE_LOOKUP[rt->kind], TYPE_LOOKUP[lt->kind]);
      ERROR_COUNTER ++;
    }
    result = type_copy(lt);
    break;
  case EXPR_CALL:
    result = type_copy(e->left->symbol->type->subtype);
    break;
  case EXPR_ARG:
    break;
  case EXPR_SUBSCRIPT:
    break;
  case EXPR_AND:
    if( lt->kind!=TYPE_BOOLEAN || rt->kind!=TYPE_BOOLEAN ) {
      printf(ERRORMSG_TYPE_EXPR_LOGICAL_AND_ERROR, EXPR_LOOKUP[EXPR_AND], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_OR:
    if( lt->kind!=TYPE_BOOLEAN || rt->kind!=TYPE_BOOLEAN ) {
      printf(ERRORMSG_TYPE_EXPR_LOGICAL_OR_ERROR, EXPR_LOOKUP[EXPR_OR], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_NOT:
    if( lt->kind!=TYPE_BOOLEAN) {
      printf(ERRORMSG_TYPE_EXPR_LOGICAL_NOT_ERROR, TYPE_LOOKUP[lt->kind], e->left->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_EXP:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf(ERRORMSG_TYPE_EXPR_LOGICAL_EXP_ERROR, EXPR_LOOKUP[EXPR_EXP], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_MOD:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf("TYPE_ERROR: `%s` can not be performed on [(%s) %s %% (%s) %s] \n", EXPR_LOOKUP[EXPR_MOD], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_LT:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf("TYPE_ERROR: `%s` can not be performed on [(%s) %s < (%s) %s] \n", EXPR_LOOKUP[EXPR_LT], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_GT:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf("TYPE_ERROR: `%s` can not be performed on [(%s) %s > (%s) %s] \n", EXPR_LOOKUP[EXPR_GT], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_LTE:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf("TYPE_ERROR: `%s` can not be performed on [(%s) %s <= (%s) %s] \n", EXPR_LOOKUP[EXPR_LTE], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_GTE:
    if( lt->kind!=TYPE_INTEGER || rt->kind!=TYPE_INTEGER ) {
      printf("TYPE_ERROR: `%s` can not be performed on [(%s) %s >= (%s) %s] \n", EXPR_LOOKUP[EXPR_GTE], TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN, 0, 0);
    break;
  case EXPR_EQ:
  case EXPR_NEQ:
    if(!type_equals(lt, rt)) {
      printf("===================================\n");
      printf("TYPE ERROR: << TYPE_MISTMATCH >>\n");
      printf("\t <Equality Operation> can not be performed when the types of operators don't match\n");
      printf("\t[(%s) %s == (%s) %s] \n", TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      printf("===================================\n");
      ERROR_COUNTER ++;
    }
    if(
      (lt->kind==TYPE_VOID || lt->kind==TYPE_ARRAY || lt->kind==TYPE_FUNCTION) 
       || 
      (rt->kind==TYPE_VOID || rt->kind==TYPE_ARRAY || rt->kind==TYPE_FUNCTION)
      ) {
      printf("===================================\n");
      printf("TYPE ERROR: << INVALID TYPE >>\n");
      printf("\t <Equality Operation> can not be performed when (%s) %s == (%s) %s \n", TYPE_LOOKUP[lt->kind], e->left->name, TYPE_LOOKUP[rt->kind], e->right->name);
      printf("===================================\n");
      ERROR_COUNTER ++;
    }
    result = type_create(TYPE_BOOLEAN,0,0);
    break;
  case EXPR_ARR:

    break;
  }
  // types are no longer needed
  type_delete(lt);
  type_delete(rt);
  return result;
}

void stmt_typecheck(struct stmt *s) {
	if (!s) return;
	switch (s->kind) {
		case STMT_BLOCK:
      stmt_typecheck(s->body);
			break;
		case STMT_DECL:
      decl_typecheck(s->decl);
			break;
    case STMT_EXPR:
      expr_typecheck(s->expr);
      break;
    case STMT_IF_ELSE:
      break;
    case STMT_IF:
      break;
    case STMT_FOR:
      break;
    case STMT_PRINT:
      break;
    case STMT_RETURN:
      return;
	}
  stmt_typecheck(s->next); // if its a statement_list
}


void decl_typecheck( struct decl *d ) {
    if (!d) return;
    if(d->value) {
        struct type *t;
        t = expr_typecheck(d->value);
        if(!type_equals(d->type, t)) {
          if(d->value->kind == EXPR_CALL) {
            printf("TYPE_ERROR: Can not assign type of (%s) to (%s) in `%s: %s = %s;`\n", 
                TYPE_LOOKUP[t->kind], 
                TYPE_LOOKUP[d->type->kind], 
                d->name, 
                TYPE_LOOKUP[d->type->kind], 
                d->value->left->name
            );
          }
          else if (d->value->kind != EXPR_NAME) {
            printf("TYPE_ERROR: Can not assign type of (%s) to (%s) in `%s: %s = %d;`\n", 
                TYPE_LOOKUP[t->kind], 
                TYPE_LOOKUP[d->type->kind], 
                d->name, 
                TYPE_LOOKUP[d->type->kind], 
                d->value->literal_value
            );

          }
          else {
            printf("TYPE_ERROR: Can not assign type of (%s) to (%s) in `%s: %s = %s;`\n", 
                TYPE_LOOKUP[t->kind], 
                TYPE_LOOKUP[d->type->kind], 
                d->name, 
                TYPE_LOOKUP[d->type->kind], 
                d->value->name
            );
          }
          ERROR_COUNTER ++;
        }
    }
    if(d->code) {
        stmt_typecheck(d->code);
    }
    decl_typecheck(d->next);
}
