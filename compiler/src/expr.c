#include "../include/expr.h"
#include <stdlib.h>

struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right) {
  struct expr *e = malloc(sizeof(*e));
  e->kind = kind;
  e->literal_value = 0;
  e->left = left;
  e->right = right;
  return e;
}

// TODO(Ali)
// NOTE(Ali): Note sure what this thing is, probably an identifier
struct expr * expr_create_name( const char *n ) { 
  struct expr* str_literal =  expr_create(EXPR_NAME, NULL, NULL);
  str_literal->name = n;
  return str_literal;
}

struct expr * expr_create_integer_literal( int c ) { 
  struct expr* int_literal =  expr_create(EXPR_INTEGER_LITERAL, NULL, NULL);
  int_literal->literal_value = c;
  return int_literal;
}

struct expr * expr_create_boolean_literal( int c ) { 
  struct expr* bool_literal =  expr_create(EXPR_BOOLEAN_LITERAL, NULL, NULL);
  bool_literal->literal_value = c;
  return bool_literal;
}

struct expr * expr_create_char_literal( char c ) { 
  struct expr* char_literal =  expr_create(EXPR_CHAR_LITERAL, NULL, NULL);
  char_literal->literal_value = c;
  return char_literal;
}

struct expr * expr_create_string_literal( const char *str ) { 
  struct expr* str_literal =  expr_create(EXPR_CHAR_LITERAL, NULL, NULL);
  str_literal->string_literal = str;
  return str_literal;
}
