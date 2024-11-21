#define _CRT_SECURE_NO_DEPRECATE
#include "../include/token.h"
#include "../include/messages.h"
#include "../include/constants.h"
#include "../include/print.h"
#include "../include/stack.h"
#include "../include/resolve.h"


extern stack_t SYMBOL_STACK;
extern void run_scan(const char *); 
extern int yyparse(); 
void run_parser(const char *);
void run_resolve(const char *);
void run_typecheck(const char *);
void run_print(struct stmt* ast);

struct decl* parser_result;
int main(int argc, char *argv[]) {
    const char *option = argv[1];
    const char *filename = argv[2];

    if (argc < 3) {
        fprintf(stderr, USAGEMSG_SCANNER,      argv[0]);
        fprintf(stderr, USAGEMSG_PARSER,       argv[0]);
        fprintf(stderr, USAGEMSG_TYPECHECKER,  argv[0]);
        fprintf(stderr, USAGEMSG_NAMERESOLVER, argv[0]);
        exit(1);
    }

    if (strcmp(option, "-scan") == 0) run_scan(filename); 
    else if (strcmp(option, "-parse") == 0) run_parser(filename); 
    else if (strcmp(option, "-resolve") == 0) run_resolve(filename); 
    else if (strcmp(option, "-typecheck") == 0) run_typecheck(filename); 
    else {
        fprintf(stderr, "Unknown option: %s\n", option);
        exit(1);
    }

    return 0;
}

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
  //struct hash_table* ht = hash_table_create(1,0);
  //stack_print(&SYMBOL_STACK);
  //stack_push(&SYMBOL_STACK, *ht);
  //stack_print(&SYMBOL_STACK);
  // traverse AST
  decl_resolve(parser_result);

  // clean the stack
  //stack_destroy(&SYMBOL_STACK);
}

void run_typecheck(const char* filename) {
  printf("running typechecker");
}

