#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/stack.h"
#include "../include/decl.h"

// flex
extern char *yytext; // Actual text scanned
extern FILE *yyin;   // Input file for the scanner to use.
extern int yyparse(); // parse functon for bison
extern int yylex();  // Scans text, returns 0 on EOF.
extern int yylineno; // Line number of the last scanned text.
extern void run_scan(const char *); 

// user defined
extern int EXIT_CODE; 
extern stack_t SYMBOL_STACK;
extern struct decl* parser_result;

#endif 
