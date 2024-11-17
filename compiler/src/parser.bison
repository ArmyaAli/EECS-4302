%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/helper.h"
#include "../include/expr.h"
#include "../include/print.h"
#include "../include/type.h"
#include "../include/stmt.h"
#include "../include/decl.h"
#include "../include/param_list.h"

extern char *yytext               ;
extern int yylex()                ;
int yyerror( char *str)           ;
extern struct stmt* parser_result ;

%}

%union {
struct decl *decl ;
struct expr *expr ;
struct type *type ;
struct stmt *stmt ;
struct param_list *param_list ;
char* str         ;
int integer_type_name         ;
}                 ;

%type <stmt> program
%type <decl> var_declaration
%type <expr> arg_list expr cond_expr term factor identifier function_call incr_decr init_expr next_expr mid_epr arr_element_list nested_array_reassign nested_sq_bracket_list
%type <stmt> print_statement return_statement for_statement statement statement_list block_statment reassignment
%type <type> type_specifier neseted_array neseted_array_list
%type <integer_type_name>  token_digit_literal
%type <param_list>  param param_list

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
program : block_statment { parser_result = $1 ; }
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
var_declaration : identifier TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_SEMICOLON 
    { 
        $$ = decl_create (
            $1->name,
            $3,
            NULL,
            NULL,
            NULL
        );
    }
    ;
| identifier TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON 
    { 
        $$ = decl_create (
            $1->name,
            $3,
            $5,
            NULL,
            NULL
        );
    }
    ;
| identifier TOKEN_TYPE_ASSIGNMENT neseted_array_list type_specifier TOKEN_SEMICOLON
    { 
        // get the last element in the linked list
        struct type* current = $3;
        struct type* head = current;
        while (current->subtype != NULL) {
            current = current->subtype;
        }
        // set the last element of linked list to have the value of type specifier - refer to page 93 to understand this
        current->subtype = $4;
        
        $$ = decl_create (
            $1->name,
            head,
            NULL,
            NULL,
            NULL
        );
    }
    ;
| identifier TOKEN_TYPE_ASSIGNMENT neseted_array_list type_specifier TOKEN_ASSIGNMENT TOKEN_OPEN_CURLY_BRACE arr_element_list TOKEN_CLOSE_CURLY_BRACE TOKEN_SEMICOLON
    { 
        // get the last element in the linked list
        struct type* current = $3;
        struct type* head = current;
        while (current->subtype != NULL) {
            current = current->subtype;
        }
        // set the last element of linked list to have the value of type specifier - refer to page 93 to understand this
        current->subtype = $4;
        
        $$ = decl_create (
            $1->name,
            head,
            $7,
            NULL,
            NULL
        );
    }
    ;
| identifier TOKEN_TYPE_ASSIGNMENT type_specifier TOKEN_ASSIGNMENT identifier nested_sq_bracket_list TOKEN_SEMICOLON
    {
        $6->left = $5;
        $$ = decl_create (
            $1->name,
            $3,
            $6,
            NULL,
            NULL
        );
    }
    ;
| identifier TOKEN_TYPE_ASSIGNMENT TOKEN_FUNCTION type_specifier TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_SEMICOLON // gfx_clear_color: function void ( red:integer, green: integer, blue:integer )    ;
;

// calc: function integer (param1: boolean, param2: integer) = {}
function_declaration : identifier TOKEN_TYPE_ASSIGNMENT TOKEN_FUNCTION type_specifier TOKEN_LPAREN param_list TOKEN_RPAREN TOKEN_ASSIGNMENT block_statment 
;

// param list can be a single / multiple param
param_list: param_list TOKEN_COMMA param_list { $$ = $1; $1->next = $3; }
| param { $$ = $1; }
|
;

// (param1: boolean) or (param1: boolean, param2: integer, ...)
param : identifier TOKEN_TYPE_ASSIGNMENT type_specifier 
{
    $$ = param_list_create(
        $1->name,
        $3,
        NULL
    );
}
| identifier TOKEN_TYPE_ASSIGNMENT neseted_array_list type_specifier
;

arr_element_list : arr_element_list TOKEN_COMMA arr_element_list { $$ = expr_create(EXPR_ARR, $1, $3) ; }
| expr { $$ = $1                                                                 ; }
| { $$ = 0                                                                       ; }
;

// a: array[] array[] ... OR a: array[3] array[2] ...
neseted_array_list : neseted_array neseted_array_list { $$ = $1; $1->subtype = $2; }
| neseted_array { $$ = $1; }
;

neseted_array : TOKEN_ARRAY TOKEN_OPEN_SQUARE_BRACE TOKEN_CLOSE_SQUARE_BRACE
{
    $$ = type_create(
        TYPE_ARRAY,
        NULL,
        NULL
    );
}
| TOKEN_ARRAY TOKEN_OPEN_SQUARE_BRACE token_digit_literal TOKEN_CLOSE_SQUARE_BRACE
{
    struct type* t = type_create(
        TYPE_ARRAY,
        NULL,
        NULL
    );
    t->array_size = $3;
    $$ = t;
}
;

// [3][3+4][2/23+4^4]...
nested_sq_bracket_list : nested_array_reassign nested_sq_bracket_list // works only for one level - no nesting 
{
    $$ = $1; $1->right = expr_create(EXPR_SUBSCRIPT, $1->right, $2->right);
}
| nested_array_reassign 
{
    $$ = $1;
}
;

nested_array_reassign : TOKEN_OPEN_SQUARE_BRACE expr TOKEN_CLOSE_SQUARE_BRACE
{
    $$ = expr_create(EXPR_SUBSCRIPT, NULL, $2);
}
;

// statement list can be a single / multiple statement
statement_list : statement_list statement { $$ = $1; $1->next = $2; }
| statement { $$ = $1; }
|
;

// statment can be either a variable declaration, if statement, block statement
statement : var_declaration { 
    $$ = stmt_create (
        STMT_DECL,
        $1,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );
 }
| reassignment { $$ = $1; }
| if_statement_list
| for_statement { $$ = $1; }
| function_call TOKEN_SEMICOLON
{
    $$ = stmt_create (
        STMT_EXPR,
        NULL,
        NULL,
        $1,
        NULL,
        NULL,
        NULL,
        NULL
    );
}
| block_statment { $$ = $1; }
| print_statement { $$ = $1; }
| return_statement { $$ = $1; }
;

reassignment : identifier TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON 
{
    struct expr* e = expr_create(EXPR_ASSIGN, expr_create_name($1->name), $3);
    $$ = stmt_create (
        STMT_EXPR,
        NULL,
        NULL,
        e,
        NULL,
        NULL,
        NULL,
        NULL
    );
};
| incr_decr TOKEN_SEMICOLON
{
    $$ = stmt_create (
        STMT_EXPR,
        NULL,
        NULL,
        $1,
        NULL,
        NULL,
        NULL,
        NULL
    );
}
| identifier nested_sq_bracket_list TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON
{
    $2->left = $1;
    struct expr* e = expr_create(EXPR_ASSIGN, $2, $4);
    $$ = stmt_create (
        STMT_EXPR,
        NULL,
        NULL,
        e,
        NULL,
        NULL,
        NULL,
        NULL
    );
}
| identifier TOKEN_ASSIGNMENT identifier nested_sq_bracket_list TOKEN_SEMICOLON // x = arr[3][4]...
| identifier nested_sq_bracket_list TOKEN_ASSIGNMENT identifier nested_sq_bracket_list TOKEN_SEMICOLON // arr[2][3]... = arr[3][3]... ;
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

for_statement : TOKEN_FOR TOKEN_LPAREN init_expr TOKEN_SEMICOLON mid_epr TOKEN_SEMICOLON next_expr TOKEN_RPAREN
{
    $$ = stmt_create (
        STMT_FOR,
        NULL,
        $3,
        $5,
        $7,
        NULL,
        NULL,
        NULL
    );
}
| TOKEN_FOR TOKEN_LPAREN init_expr TOKEN_SEMICOLON mid_epr TOKEN_SEMICOLON next_expr TOKEN_RPAREN block_statment
;

// the first part of the for loop
init_expr : identifier TOKEN_ASSIGNMENT expr { $$ = expr_create(EXPR_ASSIGN, $1, $3); }
|
;

// the conditional part of the for loop
mid_epr : cond_expr { $$ = $1; }
|
;

// the conditional decision maker part of for looop
next_expr : expr { $$ = $1; }
| incr_decr { $$ = $1; }
|
;

function_call : identifier TOKEN_LPAREN arg_list TOKEN_RPAREN { $$ = expr_create(EXPR_CALL, $1, $3) ; }
;

// indicates a block of statements inside curly braces
block_statment : TOKEN_OPEN_CURLY_BRACE statement_list TOKEN_CLOSE_CURLY_BRACE 
{
    $$ = stmt_create (
        STMT_BLOCK,
        NULL,
        NULL,
        NULL,
        NULL,
        $2,
        NULL,
        NULL
    );
}
;

print_statement : TOKEN_PRINT arg_list TOKEN_SEMICOLON 
{
    $$ = stmt_create(
        STMT_PRINT,
        NULL,
        NULL,
        $2,
        NULL,
        NULL,
        NULL,
        NULL
    )                                                         ; }
;

return_statement : TOKEN_RETURN expr TOKEN_SEMICOLON 
{
    $$ = stmt_create(
        STMT_RETURN,
        NULL,
        NULL,
        $2,
        NULL,
        NULL,
        NULL,
        NULL
    )                                                      ;
}
| TOKEN_RETURN incr_decr TOKEN_SEMICOLON 
{
    $$ = stmt_create(
        STMT_RETURN,
        NULL,
        NULL,
        $2,
        NULL,
        NULL,
        NULL,
        NULL
    )                                                      ;
}
;

incr_decr : identifier TOKEN_INCR { $$ = expr_create(EXPR_INCR, $1, NULL) ; }
| identifier TOKEN_DECR { $$ = expr_create(EXPR_DECR, $1, NULL)           ; }
;

arg_list : arg_list TOKEN_COMMA arg_list { $$ = expr_create(EXPR_ARG, $1, $3) ; }
| expr { $$ = $1                                                                 ; }
| { $$ = 0                                                                       ; }
;

// Type Specifiers (int, bool, char, string)
type_specifier : TOKEN_INTEGER { $$ = type_create(TYPE_INTEGER, NULL, NULL) ; }
| TOKEN_BOOLEAN { $$ = type_create(TYPE_BOOLEAN, NULL, NULL)                ;  }
| TOKEN_CHAR { $$ = type_create(TYPE_CHARACTER, NULL, NULL)                 ; }
| TOKEN_STRING { $$ = type_create(TYPE_STRING, NULL, NULL)                  ;    }
| TOKEN_VOID { $$ = type_create(TYPE_VOID, NULL, NULL)                      ;      }

;

// Expression Grammar
expr : expr TOKEN_ADD term {$$ = expr_create(EXPR_ADD, $1, $3) ;}
| expr TOKEN_SUB term {$$ = expr_create(EXPR_SUB, $1, $3)      ;}
| cond_expr { $$ = $1                                          ;}
| term { $$ = $1                                               ;}
;

// captures only conditional expressions. Used to seperate regular expression with conditiaonl ones
cond_expr : TOKEN_UNARY_NEGATE expr {$$ = expr_create(EXPR_NEQ, $2, NULL)                           ;}
| expr TOKEN_GT expr {$$ = expr_create(EXPR_GT, $1, $3)                                             ;}
| expr TOKEN_LT expr {$$ = expr_create(EXPR_LT, $1, $3)                                             ;}
| expr TOKEN_GE expr {$$ = expr_create(EXPR_GTE, $1, $3)                                            ;}
| expr TOKEN_LE expr {$$ = expr_create(EXPR_LTE, $1, $3)                                            ;}
| expr TOKEN_NEQ expr {$$ = expr_create(EXPR_NEQ, $1, $3)                                           ;}
| expr TOKEN_EQ expr {$$ = expr_create(EXPR_EQ, $1, $3)                                             ;}
| expr TOKEN_LOGICAL_AND expr {$$ = expr_create(EXPR_AND, $1, $3)                                   ;}
| expr TOKEN_LOGICAL_OR expr {$$ = expr_create(EXPR_OR, $1, $3)                                     ;}
| identifier { $$ = $1                                                                              ;}
;

term : term TOKEN_EXP factor {$$ = expr_create(EXPR_EXP, $1, $3) ;}
| term TOKEN_MUL factor { $$ = expr_create(EXPR_MUL, $1, $3)     ;}
| term TOKEN_DIV factor { $$ = expr_create(EXPR_DIV, $1, $3)     ;}
| function_call { $$ = $1                                        ;}
| factor { $$ = $1                                               ; }
;

// atomic tokens in b-minor
factor : TOKEN_SUB factor {$$ = expr_create(EXPR_SUB, $2, NULL)                               ;} // to be discussed and analyzed further
| TOKEN_LPAREN expr TOKEN_RPAREN { $$ = $2                                                    ;}
| TOKEN_DIGIT {$$ = expr_create_integer_literal(atoi(yytext))                                 ;}
| TOKEN_TRUE {$$ = expr_create_boolean_literal(1)                                             ;}
| TOKEN_FALSE {$$ = expr_create_boolean_literal(0)                                            ;}
| TOKEN_STRING_LITERAL {$$ = expr_create_string_literal(yytext)                               ;}
| function_call { $$ = $1                                                                     ;}
| TOKEN_CHARACTER_LITERAL {printf("%s", "factor\n"); $$ = expr_create_char_literal(yytext[0]) ;}
;

token_digit_literal : TOKEN_DIGIT { $$ = atoi(yytext); }
;

identifier: TOKEN_IDENTIFIER { $$ = expr_create_name(strdup(yytext)) ; }
;
%%

/* This function is called whenever the parser fails to parse the input */
int yyerror( char *s ) {
printf("parser.bison: parse error: %s\n",s);
exit(1)                                                                    ;
}
