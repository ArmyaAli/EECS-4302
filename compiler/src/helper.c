#include "include/helper.h"
#include "include/constants.h"
#include "include/global.h"
#include <string.h>
#include <stdlib.h>


void process_string_literal() {
  int len = strlen(yytext);
  char* copy = malloc(len * sizeof(char));
  strcpy(copy, yytext);
  strip_qoutes(copy);
  special_char_to_byte(copy);
  strcpy(yytext, copy);
  free(copy);
}

void process_char_literal() {
  int len = strlen(yytext);
  char* copy = malloc(len * sizeof(char));
  strcpy(copy, yytext);
  strip_qoutes(copy);
  strcpy(yytext, copy);
  free(copy);
}
// if we find a // in the string followed by an escape code, let's replace with the byte character
void special_char_to_byte(char* str) {
  int len = strlen(str);
  for(int i = 0; i < len && str[i] != '\0'; ++i) {
    if(i + 1 < len) {
      for(int j = 0; j < 11; ++j) {
        char code = ESCAPE_CODE[j]; 
        if(str[i] == '\\' && str[i + 1] == code)  {
          //printf("str: %s, strlen: %d, code: %c, i: %d, j: %d\n", str, len, code, i, j);
          // replace the slash with the newline byte
          char esc_byte = ESCAPE_BYTE[j];
          str[i] = esc_byte;
          // shift left from the right side
          shift_left(str, i + 1);
        }
      }
    }
  }
}

void shift_left(char* str, int at) {
  int len = strlen(str);
  if(at >= len) return;
  //printf("left: str: %s\n", str);
  for(int i = at; i < len; ++i) str[i] = str[i + 1];
}

void strip_tail(char* str) {
  int len = strlen(str);
  str[len - 1] = '\0'; // truncate @ last index
}

void strip_qoutes(char* str) {
  strip_tail(str);
  shift_left(str, 0);
}

