#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include <string.h>

typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV,
  EXPR_NAME,
  EXPR_ASSIGN,
  EXPR_INTEGER_LITERAL,
  EXPR_BOOLEAN_LITERAL,
  EXPR_CHAR_LITERAL,
  EXPR_STRING_LITERAL,
  EXPR_CALL,
  EXPR_ARG,
  EXPR_SUBSCRIPT,
	EXPR_AND,
	EXPR_OR,
	EXPR_NOT,
	EXPR_EXP,
	EXPR_MOD,
	EXPR_LT,
	EXPR_GT,
	EXPR_LTE,
	EXPR_GTE,
	EXPR_EQ,
	EXPR_NEQ,
	EXPR_INCR,
	EXPR_DECR,
	EXPR_ARR,
} expr_t;

struct expr_list {
	char *name;
	struct expr *next;
};

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	char *name;
	int literal_value;
	char * string_literal;
	struct symbol *symbol;
  int reg;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );
struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );
struct expr * expr_copy(struct expr*);

#endif
