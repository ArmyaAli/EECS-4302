#include "include/typecheck.h"
#include "include/param_list.h"
#include "include/messages.h"
#include <stdlib.h>

// helpers
struct type *type_copy(struct type *t) { return NULL; }

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
    } else if (a->kind == TYPE_ARRAY && a->kind == TYPE_ARRAY) {
      return type_equals(a->subtype, b->subtype);
    } else if (a->kind == TYPE_FUNCTION && a->kind == TYPE_FUNCTION) {
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

void type_delete(struct type *t) {}

// typecheckers
struct type *expr_typecheck(struct expr *e) {
  if (!e)
    return 0;
  struct type *lt = expr_typecheck(e->left);
  struct type *rt = expr_typecheck(e->right);
  struct type *result;

  switch (e->kind) {
  case EXPR_INTEGER_LITERAL:
    result = type_create(TYPE_INTEGER, 0, 0);
    break;
  case EXPR_STRING_LITERAL:
    result = type_create(TYPE_STRING, 0, 0);
    break;
  case EXPR_BOOLEAN_LITERAL:
    result = type_create(TYPE_STRING, 0, 0);
    break;
  case EXPR_CHAR_LITERAL:
    result = type_create(TYPE_STRING, 0, 0);
    break;
  case EXPR_ADD:
    break;
  case EXPR_SUB:
    break;
  case EXPR_MUL:
    break;
  case EXPR_DIV:
    break;
  case EXPR_NAME:
    break;
  case EXPR_ASSIGN:
    break;
  case EXPR_CALL:
    break;
  case EXPR_ARG:
    break;
  case EXPR_SUBSCRIPT:
    break;
  case EXPR_AND:
    break;
  case EXPR_OR:
    break;
  case EXPR_NOT:
    break;
  case EXPR_EXP:
    break;
  case EXPR_MOD:
    break;
  case EXPR_LT:
    break;
  case EXPR_GT:
    break;
  case EXPR_LTE:
    break;
  case EXPR_GTE:
    break;
  case EXPR_EQ:
    break;
  case EXPR_NEQ:
    break;
  case EXPR_INCR:
    break;
  case EXPR_DECR:
    break;
  case EXPR_ARR:
    break;
  }
  type_delete(lt);
  type_delete(rt);
  return result;
}

void stmt_typecheck(struct stmt *s) {}

//void decl_typecheck(struct decl * d) {
//  if (d->value) {
//    struct type *t;
//    t = expr_typecheck(d->value);
//    if (!type_equals(t, d->symbol->type)) {
//      /* display an error */
//    }
//  }
//  if (d->code) {
//    stmt_typecheck(d->code);
//  }
//}
