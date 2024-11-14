%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/helper.h"
#include "../include/expr.h"
#include "../include/print.h"

extern char *yytext               ;
extern int yylex()                ;
int yyerror( char *str)           ;
extern struct expr* parser_result ;

%}

%union {
    struct expr *expr;
    char *name;
    int int_literal;
};

%type <expr> program
%type <expr> term
%type <expr> factor

%token TOKEN_EOF 0 // enum index start
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

// The program is a list of declaration
program : term { parser_result = $1; }
;

// declaration list can be a single / multiple declaration
declaration_list : declaration_list declaration
| declaration
|
;

// declaration is either a Variable declaration or a function declaration
declaration : function_declaration
| var_declaration
;

// Variable declaration can be either initialized or uninitialized
var_declaration : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_SEMICOLON // x: boolean                                                                                                          ;
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON // x: boolean = true && false                                                                                    ;
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT neseted_array_list type_specifier TOKEN_SEMICOLON // x: array[5] boolean                                                                                              ;
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT neseted_array_list type_specifier TOKEN_ASSIGNMENT TOKEN_OPEN_CURLY_BRACE expr_list TOKEN_CLOSE_CURLY_BRACE TOKEN_SEMICOLON // x: array[5] ... boolean = {1, 2, 3, 4} ;
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_ASSIGNMENT TOKEN_IDENTIFIER nested_sq_bracket_list TOKEN_SEMICOLON // x: char = str[1][4]...                                                     ;
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_FUNCTION type_specifier TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_SEMICOLON // gfx_clear_color: function void ( red:integer, green: integer, blue:integer )    ;
;

// calc: function integer (param1: boolean, param2: integer) = {}
function_declaration : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT TOKEN_FUNCTION type_specifier TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_ASSIGNMENT block_statment
;

// param list can be a single / multiple param
param_list: param_list TOKEN_COMMA param_list
| param
|
;

// (param1: boolean) or (param1: boolean, param2: integer, ...)
param : TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT type_specifier
| TOKEN_IDENTIFIER TOKEN_TYPE_ASSIGNMENT neseted_array_list type_specifier
;

// a: array[] array[] ... OR a: array[3] array[2] ...
neseted_array_list : neseted_array_list neseted_array
| neseted_array
;

neseted_array : TOKEN_ARRAY TOKEN_OPEN_SQUARE_BRACE TOKEN_CLOSE_SQUARE_BRACE
| TOKEN_ARRAY TOKEN_OPEN_SQUARE_BRACE TOKEN_DIGIT TOKEN_CLOSE_SQUARE_BRACE
;

// [3][3+4][2/23+4^4]...
nested_sq_bracket_list : nested_sq_bracket_list nested_array_reassign
| nested_array_reassign
;

nested_array_reassign : TOKEN_OPEN_SQUARE_BRACE expr TOKEN_CLOSE_SQUARE_BRACE
;

// statement list can be a single / multiple statement
statement_list : statement_list statement
| statement
|
;

// statment can be either a variable declaration, if statement, block statement
statement : var_declaration
| reassignment
| if_statement_list
| for_statement
| function_call TOKEN_SEMICOLON
| block_statment
| print_statement
| return_statement
;

reassignment : TOKEN_IDENTIFIER TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON // x = 3 + 4                                                                ;
| TOKEN_IDENTIFIER TOKEN_INCR TOKEN_SEMICOLON // x++
| TOKEN_IDENTIFIER TOKEN_DECR TOKEN_SEMICOLON // x--
| TOKEN_IDENTIFIER nested_sq_bracket_list TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON // x[3][4][3]... = 3                                              ;
| TOKEN_IDENTIFIER TOKEN_ASSIGNMENT TOKEN_IDENTIFIER nested_sq_bracket_list TOKEN_SEMICOLON // x = arr[3][4]...
| TOKEN_IDENTIFIER nested_sq_bracket_list TOKEN_ASSIGNMENT TOKEN_IDENTIFIER nested_sq_bracket_list TOKEN_SEMICOLON // arr[2][3]... = arr[3][3]... ;
;

// if statment can be a single / multiple nested if statments
if_statement_list : if_statement_list if_statement
| if_statement
;

// i.g, if {condition} {statments}
if_statement : TOKEN_IF TOKEN_LPAREN cond_expr TOKEN_RPAREN statement_list else_statement
| TOKEN_IF TOKEN_LPAREN cond_expr TOKEN_RPAREN block_statment else_statement
;

// optional else-statments that is else followed by either statment list or a block statment
else_statement : TOKEN_ELSE statement_list
| TOKEN_ELSE block_statment
|
;

for_statement : TOKEN_FOR TOKEN_LPAREN inner_expr TOKEN_SEMICOLON mid_epr TOKEN_SEMICOLON next_expr TOKEN_RPAREN
| TOKEN_FOR TOKEN_LPAREN inner_expr TOKEN_SEMICOLON mid_epr TOKEN_SEMICOLON next_expr TOKEN_RPAREN block_statment
;

// the first part of the for loop
inner_expr : TOKEN_IDENTIFIER TOKEN_ASSIGNMENT expr
|
;

// the conditional part of the for loop
mid_epr : cond_expr
|
;

// the conditional decision maker part of for looop
next_expr : expr
| TOKEN_IDENTIFIER TOKEN_INCR
| TOKEN_IDENTIFIER TOKEN_DECR
|
;

function_call : TOKEN_IDENTIFIER TOKEN_LPAREN expr_list TOKEN_RPAREN
;

// indicates a block of statements inside curly braces
block_statment : TOKEN_OPEN_CURLY_BRACE statement_list TOKEN_CLOSE_CURLY_BRACE
;

print_statement : TOKEN_PRINT expr_list TOKEN_SEMICOLON
;

return_statement : TOKEN_RETURN expr TOKEN_SEMICOLON
| TOKEN_RETURN TOKEN_IDENTIFIER TOKEN_INCR TOKEN_SEMICOLON
| TOKEN_RETURN TOKEN_IDENTIFIER TOKEN_DECR TOKEN_SEMICOLON
;

expr_list : expr_list TOKEN_COMMA expr_list
| expr
|
;

// Type Specifiers (int, bool, char, string)
type_specifier : TOKEN_INTEGER
| TOKEN_BOOLEAN
| TOKEN_CHAR
| TOKEN_STRING
| TOKEN_VOID
;

// captures only conditional expressions. Used to seperate regular expression with conditiaonl ones
cond_expr : TOKEN_UNARY_NEGATE expr
| expr TOKEN_GT expr
| expr TOKEN_LT expr
| expr TOKEN_GE expr
| expr TOKEN_LE expr
| expr TOKEN_NEQ expr
| expr TOKEN_EQ expr
| expr TOKEN_LOGICAL_AND expr
| expr TOKEN_LOGICAL_OR expr
| function_call
| factor

// Expression Grammar
expr : expr TOKEN_ADD term
| expr TOKEN_SUB term
| cond_expr
| term
;

term : term TOKEN_EXP TOKEN_DIGIT // 3^3
| term TOKEN_MUL factor {$$ = expr_create(EXPR_MUL, $1, $3);} // 3 * 3
| term TOKEN_DIV factor {$$ = expr_create(EXPR_DIV, $1, $3);}
| function_call // func(a, c)
| factor { $$ = $1; }
;

// atomic tokens in b-minor
factor : TOKEN_SUB factor
| TOKEN_LPAREN expr TOKEN_RPAREN
| TOKEN_DIGIT {$$ = expr_create_integer_literal(atoi(yytext));}
| TOKEN_TRUE
| TOKEN_FALSE
| TOKEN_STRING_LITERAL 
| TOKEN_CHARACTER_LITERAL
| TOKEN_IDENTIFIER

%%

/* This function is called whenever the parser fails to parse the input */
int yyerror( char *s ) {
printf("parser.bison: parse error: %s\n",s);
exit(1)                                                                    ;
}
