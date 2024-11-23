#include "include/pipeline.h"
#include "include/token.h"
#include <stdlib.h>

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
  struct hash_table* ht = hash_table_create(1,0);
  //stack_push(&SYMBOL_STACK, *ht);
  //stack_push(&SYMBOL_STACK, *ht);
  //stack_push(&SYMBOL_STACK, *ht);

  //stack_pop(&SYMBOL_STACK);
  //stack_pop(&SYMBOL_STACK);
  //stack_pop(&SYMBOL_STACK);
  //stack_pop(&SYMBOL_STACK);

  //stack_print(&SYMBOL_STACK);
  // traverse AST
  decl_resolve(parser_result);

  // clean the stack
  //stack_destroy(&SYMBOL_STACK);
}

void run_typecheck(const char* filename) {
  printf("running typechecker\n");
}

