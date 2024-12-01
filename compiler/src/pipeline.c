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

void run_codegen(const char* filename) {
  run_parser(filename);
  run_resolve(filename);
  //struct symbol* sym2 = parser_result->type->params->symbol;
  //struct symbol* sym3 = parser_result->code->body->decl->symbol;

  //printf("sym: %d\n", sym->kind);
  //const char* res = symbol_codegen(sym2);
  //printf("res: %s\n", res);
  printf("===============\n");
  decl_codegen(parser_result);
  printf("===============\n");
  printf("running codegen\n");
}
