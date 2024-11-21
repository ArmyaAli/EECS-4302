#include "include/expr.h"
#include <stdlib.h>

/* Used to create a general expression where its lift and right childs are also expressions */
struct expr *expr_create(expr_t kind, struct expr *left, struct expr *right) {
  struct expr *e = malloc(sizeof(*e));
  e->kind = kind;
  e->literal_value = 0;
  e->left = left;
  e->right = right;
  return e;
}

/* Used to create identifier expression */
struct expr * expr_create_name( const char *n ) { 

  struct expr* identifier_expr =  expr_create(EXPR_NAME, NULL, NULL);
  identifier_expr->name = n;
  return identifier_expr;
}

/* Used to create integer literal expression */
struct expr * expr_create_integer_literal( int c ) { 
  struct expr* int_literal_expr =  expr_create(EXPR_INTEGER_LITERAL, NULL, NULL);
  int_literal_expr->literal_value = c;
  return int_literal_expr;
}

/* Used to create boolean literal expression */
struct expr * expr_create_boolean_literal( int c ) { 
  struct expr* bool_literal_expr =  expr_create(EXPR_BOOLEAN_LITERAL, NULL, NULL);
  bool_literal_expr->literal_value = c;
  return bool_literal_expr;
}

/* Used to create character literal expression */
struct expr * expr_create_char_literal( char c ) { 
  struct expr* char_literal_expr =  expr_create(EXPR_CHAR_LITERAL, NULL, NULL);
  char_literal_expr->literal_value = c;
  return char_literal_expr;
}

/* Used to create string literal expression */
struct expr * expr_create_string_literal( const char *str ) { 
  struct expr* str_literal_expr =  expr_create(EXPR_STRING_LITERAL, NULL, NULL);
  str_literal_expr->string_literal = malloc(sizeof(char)*strlen(str));
  strcpy(str_literal_expr->string_literal, str);
  return str_literal_expr;
}
