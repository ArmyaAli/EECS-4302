#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char ESCAPE_CODE[];
extern char ESCAPE_BYTE[]; 

// flex
extern char *yytext; // Actual text scanned
extern FILE *yyin;   // Input file for the scanner to use.
extern int yylex();  // Scans text, returns 0 on EOF.
extern int yylineno; // Line number of the last scanned text.

// user defined
extern int EXIT_CODE; 
// defined in constants.c, 1-1 mapping with token_t from token.h
extern const char* TOKEN_LOOKUP[];

#endif 
