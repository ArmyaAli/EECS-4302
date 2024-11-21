%{
#include "../include/token.h"
#include "../include/helper.h"
#include "../include/constants.h"
#include "../include/global.h"
%}

%option noinput
%option nounput
%option yylineno
%x C_COMMENT
%x CPP_COMMENT

DIGIT [0-9]
LETTER [a-zA-Z]
WHITESPACE [ \t\n\r]
ESCAPE (\\\a|\\\b|\\\f|\\\n|\\\r|\\\v|\\\|\\\'|\\\"|\\\?)
IDENTIFIER ({LETTER}|_)({LETTER}|{DIGIT}|_)*

INTEGER_LITERAL {DIGIT}+
BOOLEAN_LITERAL (true|false)
CHARACTER_LITERAL '([^'\\]|\\.)'
STRING_LITERAL \"([^"\n]*|\\["'ntr0efa\\])*\"

INTEGER_TYPE integer
BOOLEAN_TYPE boolean
CHARACTER_TYPE char
STRING_TYPE string
ARRAY_TYPE array

%%
{WHITESPACE} {}
<INITIAL>{
"//" BEGIN(C_COMMENT)   ;
"/*" BEGIN(CPP_COMMENT) ;
}
<C_COMMENT>{
"\n" BEGIN(INITIAL)     ;
[^\n]+ // eat comment
}
<CPP_COMMENT>{
[^\*]+ // eat comment
"*"
"*/" BEGIN(INITIAL)     ;
<<EOF>> EXIT_CODE=1     ; return TOKEN_EOF;
}

array { return TOKEN_ARRAY       ;    }
boolean { return TOKEN_BOOLEAN   ;  }
char { return TOKEN_CHAR         ;     }
else { return TOKEN_ELSE         ;     }
false { return TOKEN_FALSE       ;    }
for { return TOKEN_FOR           ;      }
function { return TOKEN_FUNCTION ; }
if { return TOKEN_IF             ;       }
integer { return TOKEN_INTEGER   ;  }
print { return TOKEN_PRINT       ;    }
return { return TOKEN_RETURN     ;   }
string { return TOKEN_STRING     ;   }
true { return TOKEN_TRUE         ;     }
void { return TOKEN_VOID         ;     }
while { return TOKEN_WHILE       ;    }

";" { return TOKEN_SEMICOLON ; }
">=" { return TOKEN_GE       ; }
"<=" { return TOKEN_LE       ; }
"==" { return TOKEN_EQ       ; }
"!=" { return TOKEN_NEQ      ; }
"<" { return TOKEN_LT        ; }
">" { return TOKEN_GT        ; }
"%" { return TOKEN_MOD       ; }

"+" { return TOKEN_ADD ; }

"--" { return TOKEN_DECR ; }
"++" { return TOKEN_INCR ; }

"-" { return TOKEN_SUB        ; }
"*" { return TOKEN_MUL        ; }
"/" { return TOKEN_DIV        ; }
"^" { return TOKEN_EXP        ; }
"=" { return TOKEN_ASSIGNMENT ; }

"!" { return TOKEN_UNARY_NEGATE ; }

"(" { return TOKEN_LPAREN             ; }
")" { return TOKEN_RPAREN             ; }
"{" { return TOKEN_OPEN_CURLY_BRACE   ; }
"}" { return TOKEN_CLOSE_CURLY_BRACE  ; }
"[" { return TOKEN_OPEN_SQUARE_BRACE  ; }
"]" { return TOKEN_CLOSE_SQUARE_BRACE ; }

"," { return TOKEN_COMMA           ; }
":" { return TOKEN_TYPE_ASSIGNMENT ; }
"&&" { return TOKEN_LOGICAL_AND    ; }
"||" { return TOKEN_LOGICAL_OR     ; }

{INTEGER_LITERAL} { return TOKEN_DIGIT ; }

{CHARACTER_LITERAL} {
if (yytext[1] < 0 || yytext[1] > 255) {
return TOKEN_ERROR                      ;
}

process_char_literal() ;

return TOKEN_CHARACTER_LITERAL ;
}

{STRING_LITERAL} {

process_string_literal() ;

if (strlen(yytext) > 256) {
return TOKEN_ERROR          ;
}

return TOKEN_STRING_LITERAL ;
}

{IDENTIFIER} {
if (strlen(yytext) > 256) {
return TOKEN_ERROR          ;
}
return TOKEN_IDENTIFIER     ;
}

. { return TOKEN_ERROR  ; }
%%
int yywrap() { return 1 ; }
