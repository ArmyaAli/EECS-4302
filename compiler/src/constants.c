const char* SCRATCH_LOOKUP[17] = {
    "rbp", // base pointer
    "rsp", // stack pointer
    "rip", // instruc pointer
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rsi",
    "rdi",
    "r8", 
    "r9", 
    "r10", 
    "r11", 
    "r12", 
    "r13", 
    "r14", 
    "r15"
};

const char* SCOPE_LOOKUP[] = {
  "local",
  "param",
  "global"
};

const char* TYPE_LOOKUP[] = {
	"TYPE_VOID",
	"TYPE_BOOLEAN",
	"TYPE_CHARACTER",
	"TYPE_INTEGER",
	"TYPE_STRING",
	"TYPE_ARRAY",
	"TYPE_FUNCTION",
};

const char* EXPR_LOOKUP[] = {
     "ADDITION",
     "SUBTRACTION",
     "MULTIPLICATION",
     "DIVISION",
     "IDENTIFIER",
     "ASSIGNMENT",
     "INTEGER LITERAL",
     "BOOLEAN LITERAL",
     "CHARACTER LITERAL",
     "STRING LITERAL",
     "EXPR_CALL",
     "EXPR_ARG",
     "EXPR_SUBSCRIPT",
     "&&",
     "||",
     "!",
     "^",
     "%%",
     "<",
     ">",
     "<=",
     ">=",
     "==",
     "!=",
     "++",
     "--",
     "ARRAY"
};


// program exit code constant
// token.h token_t -> string map
const char* TOKEN_LOOKUP[] = {
     "TOKEN_EOF", // enum index start 0
     "TOKEN_SEMICOLON",
     "TOKEN_DIGIT",
     "TOKEN_CHARACTER_LITERAL",
     "TOKEN_INTEGER_ASSINGMENT",
     "TOKEN_BOOLEAN_ASSIGNMENT",
     "TOKEN_CHARACTER_ASSIGNMENT",
     "TOKEN_STRING_ASSIGNMENT",
     "TOKEN_STRING_LITERAL",
     "TOKEN_INTEGER_ARRAY",
     "TOKEN_BOOLEAN_ARRAY",
     "TOKEN_CHARACTER_ARRAY",
     "TOKEN_STRING_ARRAY",
     "TOKEN_ARRAY",
     "TOKEN_BOOLEAN",
     "TOKEN_CHAR",
     "TOKEN_ELSE",
     "TOKEN_FALSE",
     "TOKEN_FOR",
     "TOKEN_FUNCTION",
     "TOKEN_IF",
     "TOKEN_INTEGER",
     "TOKEN_PRINT",
     "TOKEN_RETURN", 
     "TOKEN_STRING", 
     "TOKEN_TRUE",
     "TOKEN_VOID",
     "TOKEN_WHILE",
     "TOKEN_IDENTIFIER",
     "TOKEN_GE", 
     "TOKEN_LE", 
     "TOKEN_EQ", 
     "TOKEN_NEQ",
     "TOKEN_GT",
     "TOKEN_LT",
     "TOKEN_MOD",
     "TOKEN_DIV",
     "TOKEN_MUL",
     "TOKEN_ADD",
     "TOKEN_SUB",
     "TOKEN_DECR",
     "TOKEN_INCR",
     "TOKEN_EXP",
     "TOKEN_ASSIGNMENT",
     "TOKEN_LPAREN",
     "TOKEN_RPAREN",
     "TOKEN_OPEN_CURLY_BRACE",
     "TOKEN_CLOSE_CURLY_BRACE",
     "TOKEN_OPEN_SQUARE_BRACE",
     "TOKEN_CLOSE_SQUARE_BRACE",
     "TOKEN_COMMA",
     "TOKEN_TYPE_ASSIGNMENT",
     "TOKEN_LOGICAL_AND",
     "TOKEN_LOGICAL_OR",
     "TOKEN_UNARY_NEGATE",
     "TOKEN_ERROR",
};

const char ESCAPE_BYTE[] = {
     '\b',
     '\a',
     ' ',
     '\f',	
     '\n',	
     '\r',
     '\t',	
     '\v',
     '\\',	
     '\'',
     '\"',	
     '\?'
};

// first 12 escape code
const char ESCAPE_CODE[] = {
     'b',
     'a',
     'e',
     'f',	
     'n',	
     'r',
     't',	
     'v',
     '\\',	
     '\'',
     '"',	
     '?'
};
