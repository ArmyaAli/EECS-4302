#ifndef GLOBAL_H
#define GLOBAL_H

#include "stack.h"
#include "codegen.h"
#include <stdio.h>

// flex
extern char *yytext; // Actual text scanned
extern FILE *yyin;   // Input file for the scanner to use.
extern int yyparse(); // parse functon for bison
extern int yylex();  // Scans text, returns 0 on EOF.
extern int yylineno; // Line number of the last scanned text.

// user defined
extern int EXIT_CODE; 
extern stack_t SYMBOL_STACK;
extern struct decl* parser_result;
extern int ERROR_COUNTER;

#endif 
