#ifndef TOKEN_H
#define TOKEN_H


typedef enum
{
     TOKEN_EOF = 0, // enum index start 0
     TOKEN_SEMICOLON, // enum index start 0
     TOKEN_DIGIT,   
     TOKEN_CHARACTER_LITERAL,
     TOKEN_INTEGER_ASSINGMENT,
     TOKEN_BOOLEAN_ASSIGNMENT,
     TOKEN_CHARACTER_ASSIGNMENT,
     TOKEN_STRING_ASSIGNMENT,
     TOKEN_STRING_LITERAL,
     TOKEN_INTEGER_ARRAY,
     TOKEN_BOOLEAN_ARRAY,
     TOKEN_CHARACTER_ARRAY,
     TOKEN_STRING_ARRAY,
     TOKEN_ARRAY,
     TOKEN_BOOLEAN,
     TOKEN_CHAR,
     TOKEN_ELSE,
     TOKEN_FALSE,
     TOKEN_FOR,
     TOKEN_FUNCTION,
     TOKEN_IF,
     TOKEN_INTEGER,
     TOKEN_PRINT,
     TOKEN_RETURN,
     TOKEN_STRING,
     TOKEN_TRUE,
     TOKEN_VOID,
     TOKEN_WHILE,
     TOKEN_IDENTIFIER,
     TOKEN_GE, 
     TOKEN_LE, 
     TOKEN_EQ, 
     TOKEN_NEQ,
     TOKEN_GT,
     TOKEN_LT,
     TOKEN_MOD,
     TOKEN_DIV,
     TOKEN_MUL,
     TOKEN_ADD,
     TOKEN_SUB,
     TOKEN_DECR,
     TOKEN_INCR,
     TOKEN_EXP,
     TOKEN_ASSIGNMENT,
     TOKEN_OPEN_ROUND_BRACE,
     TOKEN_CLOSE_ROUND_BRACE,
     TOKEN_OPEN_CURLY_BRACE,
     TOKEN_CLOSE_CURLY_BRACE,
     TOKEN_OPEN_SQUARE_BRACE,
     TOKEN_CLOSE_SQUARE_BRACE,
     TOKEN_COMMA,
     TOKEN_TYPE_ASSIGNMENT,
     TOKEN_LOGICAL_AND,
     TOKEN_LOGICAL_OR,
     TOKEN_ERROR
} token_t;

void run_scan(const char *filename);
#endif
