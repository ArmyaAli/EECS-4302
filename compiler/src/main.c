#define _CRT_SECURE_NO_DEPRECATE
#include "../include/token.h"
#include "../include/messages.h"
#include "../include/constants.h"
#include "../include/print.h"
#include "../include/expr.h"


extern void run_scan(const char *); 
extern int yyparse(); 
void run_parser(const char *);
void run_print(struct stmt* ast);
struct stmt* parser_result;

int main(int argc, char *argv[]) {
    const char *option = argv[1];
    const char *filename = argv[2];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s -scan sourcefile.bminor\n", argv[0]);
        fprintf(stderr, "Usage: %s -parse sourcefile.bminor \n", argv[0]);
        exit(1);
    }

    if (strcmp(option, "-scan") == 0) run_scan(filename); 
    else if (strcmp(option, "-parse") == 0) run_parser(filename); 
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
           //fprintf(stdout, "%d\n", token);
          } else {
           fprintf(stdout, "%s\n", TOKEN_LOOKUP[token]);
           //fprintf(stdout, "%d\n", token);
          }
        }
    }

    fclose(yyin);
}

void run_parser(const char* filename) {
    printf("running parser\n");
    yyin = fopen(filename, "r");

    if(yyparse() == 0) {
       printf("Parse success\n");
    //   expr_print(parser_result, 4, 0);
      stmt_print(parser_result, 0, 0);
    //    type_print(parser_result);
    //    decl_print(parser_result);
        // param_list_print(parser_result);
    }

    fclose(yyin);
}
