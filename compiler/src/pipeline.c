#include <stdlib.h>

#include "include/global.h"
#include "include/messages.h"
#include "include/constants.h"
#include "include/print.h"
#include "include/stack.h"
#include "include/resolve.h"
#include "include/pipeline.h"
#include "include/token.h"
#include "include/typecheck.h"
#include "include/codegen.h"
#include "include/codegen_helper.h"


struct hash_table* label_to_str;
void run_scan(const char *filename) {
    // Open the file for reading
    yyin = fopen(filename, "r");
    printf("Scanning\n");
    
    // while not EOF
    int token = -1;
    while((token = yylex()) != 0) {
      if(token == TOKEN_ERROR) {
        fprintf(stderr, ERRORMSG_SCANNER_UNEXPECTED_CHAR, yytext, yylineno);
        exit(1);
      } else {
          if ( token == TOKEN_INTEGER || 
               token == TOKEN_STRING_LITERAL || 
               token == TOKEN_CHARACTER_LITERAL || 
               token == TOKEN_IDENTIFIER) {
           fprintf(stdout, "%s: %s\n", TOKEN_LOOKUP[token], yytext);
          } else {
           fprintf(stdout, "%s\n", TOKEN_LOOKUP[token]);
          }
        }
    }

    fclose(yyin);
}

void run_parser(const char* filename) {
    yyin = fopen(filename, "r");
    if(yyparse() == 0) {
       printf("Parse success\n");
       decl_print(parser_result);
    }
    fclose(yyin);
}

// We have something leaking memory
// OR we have some Undefined behaviour
void run_resolve(const char* filename) {
  run_parser(filename);

  // init our stack
  stack_init(&SYMBOL_STACK);

  //stack_print(&SYMBOL_STACK);
  // traverse AST
  decl_resolve(parser_result);

  // clean the stack
  //stack_destroy(&SYMBOL_STACK);
}

void run_typecheck(const char* filename) {
  run_parser(filename);
  run_resolve(filename);

  printf("===========================================\n");
  decl_typecheck(parser_result);
  if (ERROR_COUNTER != 0) {
    printf("TOTAL_ERRORS: %d\n", ERROR_COUNTER);
  }
  else {
    printf("Program compiled with no compilation error :)\n");
  }
  printf("===========================================\n");

  printf("running typechecker\n");
}

// First pass on AST handles all global decls and string literals
// Second pass will handle global functions and .text section (.text section is the function body)
// Step 1: Generate label .file
// Step 2: Generate data label
//  - First pass of AST
//  - Generate next label avalible, starts @ 0
//  - For string literals, generate .string label with string literal
// Step 3: Generate .text, this will be second Pass
//  - Second pass handles the following cases: 
//    - Handles functiosn on the global scope, creates a .global label with the function name of the respective function
//    - Alongside it will generate the .text section 
void run_codegen(const char* filename) {
  // Allocate our hash table for labels
  label_to_str = hash_table_create(1, 0);
  run_parser(filename);
  run_resolve(filename);
  // Step 1
  printf(".file\t\"%s\"\n", filename);
  // Step 2
  // Generate the .data label
  printf(".data\n");
  // Perform first pass on AST
  first_pass(parser_result);
}
