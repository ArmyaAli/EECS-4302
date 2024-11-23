#include "include/typecheck.h"
#include <stdlib.h>

// helpers
struct type *type_copy(struct type *t) { return NULL; }
int type_equals(struct type *a, struct type *b) { return 0; }
void type_delete(struct type *t) {}

// typecheckers
struct type *expr_typecheck(struct expr *e) {
  if (!e) return 0;
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
void decl_typecheck(struct decl *d) {}
