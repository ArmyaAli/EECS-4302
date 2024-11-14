/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INCLUDE_TOKEN_H_INCLUDED
# define YY_YY_INCLUDE_TOKEN_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_EOF = 0,
    TOKEN_SEMICOLON = 1,
    TOKEN_DIGIT = 2,
    TOKEN_CHARACTER_LITERAL = 3,
    TOKEN_INTEGER_ASSINGMENT = 4,
    TOKEN_BOOLEAN_ASSIGNMENT = 5,
    TOKEN_CHARACTER_ASSIGNMENT = 6,
    TOKEN_STRING_ASSIGNMENT = 7,
    TOKEN_STRING_LITERAL = 8,
    TOKEN_INTEGER_ARRAY = 9,
    TOKEN_BOOLEAN_ARRAY = 10,
    TOKEN_CHARACTER_ARRAY = 11,
    TOKEN_STRING_ARRAY = 12,
    TOKEN_ARRAY = 13,
    TOKEN_BOOLEAN = 14,
    TOKEN_CHAR = 15,
    TOKEN_ELSE = 16,
    TOKEN_FALSE = 17,
    TOKEN_FOR = 18,
    TOKEN_FUNCTION = 19,
    TOKEN_IF = 20,
    TOKEN_INTEGER = 21,
    TOKEN_PRINT = 22,
    TOKEN_RETURN = 23,
    TOKEN_STRING = 24,
    TOKEN_TRUE = 25,
    TOKEN_VOID = 26,
    TOKEN_WHILE = 27,
    TOKEN_IDENTIFIER = 28,
    TOKEN_GE = 29,
    TOKEN_LE = 30,
    TOKEN_EQ = 31,
    TOKEN_NEQ = 32,
    TOKEN_GT = 33,
    TOKEN_LT = 34,
    TOKEN_MOD = 35,
    TOKEN_DIV = 36,
    TOKEN_MUL = 37,
    TOKEN_ADD = 38,
    TOKEN_SUB = 39,
    TOKEN_DECR = 40,
    TOKEN_INCR = 41,
    TOKEN_EXP = 42,
    TOKEN_ASSIGNMENT = 43,
    TOKEN_LPAREN = 44,
    TOKEN_RPAREN = 45,
    TOKEN_OPEN_CURLY_BRACE = 46,
    TOKEN_CLOSE_CURLY_BRACE = 47,
    TOKEN_OPEN_SQUARE_BRACE = 48,
    TOKEN_CLOSE_SQUARE_BRACE = 49,
    TOKEN_COMMA = 50,
    TOKEN_TYPE_ASSIGNMENT = 51,
    TOKEN_LOGICAL_AND = 52,
    TOKEN_LOGICAL_OR = 53,
    TOKEN_UNARY_NEGATE = 54,
    TOKEN_ERROR = 55
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 16 "src/parser.bison" /* yacc.c:1909  */

    struct expr *expr;
    char *name;
    int int_literal;

#line 119 "include/token.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_TOKEN_H_INCLUDED  */
