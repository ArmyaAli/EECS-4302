%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/helper.h"
#include "../include/expr.h"
#include "../include/print.h"

extern char *yytext     ;
extern int yylex()      ;
int yyerror( char *str) ;

%}
%token TOKEN_EOF 0 // enum index start 0
%token TOKEN_SEMICOLON 1
%token TOKEN_DIGIT 2
%token TOKEN_CHARACTER_LITERAL 3
%token TOKEN_INTEGER_ASSINGMENT 4
%token TOKEN_BOOLEAN_ASSIGNMENT 5
%token TOKEN_CHARACTER_ASSIGNMENT 6
%token TOKEN_STRING_ASSIGNMENT 7
%token TOKEN_STRING_LITERAL 8
%token TOKEN_INTEGER_ARRAY 9
%token TOKEN_BOOLEAN_ARRAY 10
%token TOKEN_CHARACTER_ARRAY 11
%token TOKEN_STRING_ARRAY 12
%token TOKEN_ARRAY 13
%token TOKEN_BOOLEAN 14
%token TOKEN_CHAR 15
%token TOKEN_ELSE 16
%token TOKEN_FALSE 17
%token TOKEN_FOR 18
%token TOKEN_FUNCTION 19
%token TOKEN_IF 20
%token TOKEN_INTEGER 21
%token TOKEN_PRINT 22
%token TOKEN_RETURN 23
%token TOKEN_STRING 24
%token TOKEN_TRUE 25
%token TOKEN_VOID 26
%token TOKEN_WHILE 27
%token TOKEN_IDENTIFIER 28
%token TOKEN_GE 29
%token TOKEN_LE 30
%token TOKEN_EQ 31
%token TOKEN_NEQ 32
%token TOKEN_GT 33
%token TOKEN_LT 34
%token TOKEN_MOD 35
%token TOKEN_DIV 36
%token TOKEN_MUL 37
%token TOKEN_ADD 38
%token TOKEN_SUB 39
%token TOKEN_DECR 40
%token TOKEN_INCR 41
%token TOKEN_EXP 42
%token TOKEN_ASSIGNMENT 43
%token TOKEN_LPAREN 44
%token TOKEN_RPAREN 45
%token TOKEN_OPEN_CURLY_BRACE 46
%token TOKEN_CLOSE_CURLY_BRACE 47
%token TOKEN_OPEN_SQUARE_BRACE 48
%token TOKEN_CLOSE_SQUARE_BRACE 49
%token TOKEN_COMMA 50
%token TOKEN_TYPE_ASSIGNMENT 51
%token TOKEN_LOGICAL_AND 52
%token TOKEN_LOGICAL_OR 53
%token TOKEN_UNARY_NEGATE 54
%token TOKEN_ERROR 55
%%

// The program can either be a series of declarations or an expression followed by a semicolon.
program : statement_list
;

statement_list : statement_list statement
| statement
|
;

// declaration_list : declaration_list var_declaration { printf("VAR DECLARATION 1\n"); }
// | var_declaration
//                                                                                        ;

function_call: TOKEN_IDENTIFIER TOKEN_LPAREN arg_list TOKEN_RPAREN TOKEN_SEMICOLON
;

arg_list: arg_list TOKEN_COMMA arg_list
| arg
|
;

arg : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT type_specifier
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_ARRAY TOKEN_OPEN_SQUARE_BRACE TOKEN_CLOSE_SQUARE_BRACE type_specifier
;

// function declaration
function_declaration: TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_FUNCTION type_specifier TOKEN_LPAREN arg_list TOKEN_RPAREN TOKEN_ASSIGNMENT TOKEN_OPEN_CURLY_BRACE statement_list TOKEN_CLOSE_CURLY_BRACE
;

statement : if_statement
| if_else_statement
| block_statement
| var_declaration
| function_declaration
| var_reassign
| function_call
| print
| for_statement
| incr_statement TOKEN_SEMICOLON
| decr_statement
| return_statement
| expr TOKEN_SEMICOLON
;

return_statement : TOKEN_RETURN expr TOKEN_SEMICOLON
| TOKEN_RETURN TOKEN_SEMICOLON
;

if_statement : TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN TOKEN_OPEN_CURLY_BRACE statement_list TOKEN_CLOSE_CURLY_BRACE
| TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN statement_list
;

if_else_statement : if_statement TOKEN_ELSE block_statement
| if_statement TOKEN_ELSE statement_list
| if_statement else_if_list
| if_statement else_if_list TOKEN_ELSE block_statement
| if_statement else_if_list TOKEN_ELSE statement_list
;

else_if_list : else_if_list else_if
| else_if
;

else_if : TOKEN_ELSE TOKEN_IF block_statement
| TOKEN_ELSE TOKEN_IF statement_list
;

block_statement : TOKEN_OPEN_CURLY_BRACE statement_list TOKEN_CLOSE_CURLY_BRACE
;

for_statement : TOKEN_FOR TOKEN_LPAREN TOKEN_SEMICOLON TOKEN_SEMICOLON TOKEN_RPAREN TOKEN_OPEN_CURLY_BRACE statement_list TOKEN_CLOSE_CURLY_BRACE
| TOKEN_FOR TOKEN_LPAREN TOKEN_SEMICOLON TOKEN_SEMICOLON TOKEN_RPAREN statement_list
| TOKEN_FOR TOKEN_LPAREN for_arg_list TOKEN_RPAREN TOKEN_OPEN_CURLY_BRACE statement_list TOKEN_CLOSE_CURLY_BRACE
| TOKEN_FOR TOKEN_LPAREN for_arg_list TOKEN_RPAREN statement_list
;
for_arg_list : for_arg expr TOKEN_SEMICOLON for_arg
| for_arg expr TOKEN_SEMICOLON TOKEN_IDENTIFIER TOKEN_INCR
| for_arg expr TOKEN_SEMICOLON TOKEN_IDENTIFIER TOKEN_DECR
;

for_arg : TOKEN_IDENTIFIER TOKEN_ASSIGNMENT factor TOKEN_SEMICOLON
;

// Variable Declarations without an assignment
var_declaration : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_SEMICOLON
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_ARRAY TOKEN_OPEN_SQUARE_BRACE TOKEN_DIGIT TOKEN_CLOSE_SQUARE_BRACE type_specifier TOKEN_SEMICOLON // a: array[4] integer ;
| var_declaration_array_with_assign
| var_declaration_with_assign
;

array_decl : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_OPEN_SQUARE_BRACE TOKEN_CLOSE_SQUARE_BRACE type_specifier
;
array_decl_2d : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_OPEN_SQUARE_BRACE TOKEN_CLOSE_SQUARE_BRACE TOKEN_TYPE_ASSIGNMENT TOKEN_OPEN_SQUARE_BRACE TOKEN_CLOSE_SQUARE_BRACE type_specifier
;
array_decl_assign :
;

var_declaration_array_with_assign : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_ARRAY TOKEN_OPEN_SQUARE_BRACE TOKEN_DIGIT TOKEN_CLOSE_SQUARE_BRACE type_specifier TOKEN_ASSIGNMENT TOKEN_OPEN_CURLY_BRACE expr_list TOKEN_CLOSE_CURLY_BRACE TOKEN_SEMICOLON // a: array[4] integer = {1, 2, 3, 4} ;

// Variable Declarations with an assignment
var_declaration_with_assign : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON
;
var_reassign : TOKEN_IDENTIFIER TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON
;
incr_statement : TOKEN_IDENTIFIER TOKEN_INCR
;
decr_statement : TOKEN_IDENTIFIER TOKEN_DECR TOKEN_SEMICOLON
;

expr_list : expr
| expr_list TOKEN_COMMA expr
|
;

// Type Specifiers (int, bool, char, string)
type_specifier : TOKEN_INTEGER
| TOKEN_BOOLEAN
| TOKEN_CHAR
| TOKEN_STRING
| TOKEN_VOID
;

// Expression Grammar
expr : expr TOKEN_ADD term
| expr TOKEN_SUB term
| TOKEN_UNARY_NEGATE expr
| incr_statement
| term
;

// Term Grammar (multiplication and division)
term : term TOKEN_EXP TOKEN_DIGIT
| term TOKEN_MUL factor
| term TOKEN_DIV factor
| cond_term
| factor
;

cond_term : term TOKEN_GT factor
| term TOKEN_LT factor
| term TOKEN_GE factor
| term TOKEN_LE factor
| term TOKEN_NEQ factor
| term TOKEN_EQ factor
| term TOKEN_LOGICAL_AND factor
| term TOKEN_LOGICAL_OR factor

factor_list : factor_list TOKEN_COMMA factor_list
| factor
;

// Factor Grammar (negation, parentheses, and digit handling)
factor : TOKEN_SUB factor
| TOKEN_LPAREN expr TOKEN_RPAREN
| TOKEN_DIGIT
| TOKEN_TRUE
| TOKEN_FALSE
| TOKEN_STRING_LITERAL
| TOKEN_CHARACTER_LITERAL
| TOKEN_IDENTIFIER
;

print : TOKEN_PRINT factor_list TOKEN_SEMICOLON
;

%%

/* This function is called whenever the parser fails to parse the input */
int yyerror( char *s ) {
printf("parser.bison: parse error: %s\n",s);
exit(1)                                                                    ;
}
