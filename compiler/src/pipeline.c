#include <stdlib.h>

#include "include/global.h"
#include "include/messages.h"
#include "include/constants.h"
#include "include/print.h"
#include "include/stack.h"
#include "include/resolve.h"
#include "include/pipeline.h"
#include "include/type.h"
#include "include/token.h"
#include "include/typecheck.h"

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
  // struct hash_table* ht = hash_table_create(1,0);
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
  run_parser(filename);
  run_resolve(filename);

  struct decl* d1 = parser_result;
  struct type* t = expr_typecheck(d1->next->next->value);
  printf("TYPE << %s >> \n", TYPE_LOOKUP[t->kind]);

  // struct decl* d1 = parser_result;
  // struct type* t = expr_typecheck(d1->code->body->next->next->expr);
  // printf("TYPE << %s >> \n", TYPE_LOOKUP[t->kind]);

//  struct decl* d2 = parser_result->next;
//   struct type* t1 = d1->code->body->decl->type;
// //  struct type* t2 = d2->type;
//   struct type* t3 = type_copy(t1);
//   int output = type_equals(t1, t3);
//   printf("output: %d\n", output);
  printf("running typechecker\n");
}

