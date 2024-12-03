#include "include/codegen.h"
#include "include/codegen_helper.h"
#include <stdio.h>

extern struct hash_table* label_to_str;

typedef struct frame {
  char reg[5];
  char var_name[10];
  int offset;
} frame_t;

frame_t stack[1000]; // will store variables that have registers
int sp = 0;

extern struct hash_table* label_to_str;

void expr_codegen(struct expr *e) {
  if (!e) return;

  switch (e->kind) {
    case EXPR_NAME:
      printf("\tCODE_GEN_EXPRNAME\n");

      int should_add = 1;
      for(int i = 0; i < sp; ++i) {
        if(strcmp(e->name, stack[i].var_name) == 0) {
          should_add = 0;
          break;
        }
      }

      // allocate a new register only if previous allocations haven't been made
      if(should_add) {
        e->reg = scratch_alloc();
        frame_t f;
        strcpy(f.reg, scratch_name(e->reg));
        strcpy(f.var_name, e->name);
        f.offset = (8 * e->symbol->which) + 8;
        stack[sp] = f;
        ++sp;
        if (e->symbol->kind == SYMBOL_GLOBAL) {
          printf("LEAQ %s, %%%s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
        }
        else {
          printf("MOVQ %s, %%%s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
        }
      }
      break;
    case EXPR_INTEGER_LITERAL:
      printf("\tCODE_GEN_INT\n");
      e->reg = scratch_alloc();
      printf("MOVQ $%d, %%%s\n", e->literal_value, scratch_name(e->reg));
      break;
    case EXPR_STRING_LITERAL:
      printf("\tCODE_GEN_STRING\n");
      e->reg = scratch_alloc();
      break;
    case EXPR_CHAR_LITERAL:
      printf("\tCODE_GEN_CHAR\n");
      e->reg = scratch_alloc();
      printf("MOVQ $%c, %%%s\n", e->literal_value, scratch_name(e->reg));
      break;
    case EXPR_BOOLEAN_LITERAL:
      printf("\tCODE_GEN_BOOLEAN\n");
      e->reg = scratch_alloc();
      printf("MOVQ $%s, %%%s\n", e->literal_value == 0 ? "False" : "True", scratch_name(e->reg));
      break;
    case EXPR_ADD:
      printf("\tCODE_GEN_ADD\n");
      expr_codegen(e->left);
      expr_codegen(e->right);
      printf("ADDQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
      e->reg = e->right->reg;
      scratch_free(e->left->reg);
      break;
    case EXPR_SUB:
      printf("\tCODE_GEN_SUB\n");
      expr_codegen(e->left);
      expr_codegen(e->right);
      printf("SUBQ %%%s, %%%s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
      e->reg = e->left->reg;
      scratch_free(e->right->reg);
      break;
    case EXPR_MUL: // to be discussed later
      printf("\tCODE_GEN_MUL\n");
      expr_codegen(e->left);
      expr_codegen(e->right);
      printf("IMUL %%%s \n", scratch_name(e->right->reg));
      e->reg = e->right->reg;
      scratch_free(e->left->reg);
      break;
    case EXPR_DIV: // to be discussed later
      printf("\tCODE_GEN_DIV\n");
      // expr_codegen(e->left);
      // expr_codegen(e->right);
      // printf("DIV %s, %s\n", scratch_name(e->left->reg),
      //       scratch_name(e->right->reg));
      // e->reg = e->right->reg;
      // scratch_free(e->left->reg);
      break;

    case EXPR_ASSIGN:
      printf("\tCODE_GEN_EXPR_ASSIGN\n");
      expr_codegen(e->right);
      printf("MOVQ %%%s, %s\n", scratch_name(e->right->reg), symbol_codegen(e->left->symbol));
      break;
    case EXPR_CALL:
      break;
    case EXPR_ARG:
      break;
    case EXPR_SUBSCRIPT:
      break;
    
    case EXPR_AND:
      printf("\tCODE_GEN_EXPR_AND\n");
      break;
    case EXPR_OR:break;
    case EXPR_NOT:break;
    case EXPR_EXP:break;
    case EXPR_MOD:break;

    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
      printf("\tCODE_GEN_EXPR_GT\n");
      expr_codegen(e->left);
      expr_codegen(e->right);
      printf("CMPQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
      break;

    case EXPR_EQ:break;
    case EXPR_NEQ:break;
    case EXPR_INCR:
      printf("\tCODE_GEN_EXPR_INCR\n");
      printf("INCQ %%%s\n", scratch_name(e->left->reg));
      break;
    case EXPR_DECR:
      printf("\tCODE_GEN_EXPR_DECR\n");
      printf("DECQ %%%s\n", scratch_name(e->left->reg));
      break;

    case EXPR_ARR:break;
  }
}

void stmt_codegen(struct stmt *s) {
 if (!s) return;
 switch (s->kind) {
 	case STMT_BLOCK:
     printf("\tSTMT_BLOCK\n");
     stmt_codegen(s->body);
 		break;
 	case STMT_DECL:
     printf("\tSTMT_DECL\n");
     decl_codegen(s->decl);
 		break;
   case STMT_EXPR:
     printf("\tSTMT_EXPR\n");
     expr_codegen(s->expr);
     break;
   case STMT_IF_ELSE:
     printf("STMT_IF_ELSE\n");
     break;
   case STMT_IF:
     printf("STMT_IF\n");
     break;
   case STMT_FOR:
     printf("STMT_FOR\n");
     break;
   case STMT_PRINT:
     printf("STMT_PRINT\n");
     break;
   case STMT_RETURN:
     printf("STMT_RETURN\n");
     return;
 }
 stmt_codegen(s->next);
}

void decl_codegen(struct decl *d) {
	if(!d) return;

 if(d->value) {
   printf("\tDECL_VALUE\n");
   if(d->value->kind == EXPR_CALL) {
     expr_codegen(d->value->left);
   } else {
     expr_codegen(d->value);
   }
   printf("MOVQ %%%s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
 }

	if(d->code) {
     printf("\tDECL_FUNC\n");
     stmt_codegen(d->code);
 }
	decl_codegen(d->next);
}

void second_pass(struct decl *d) {
  printf("second pass\n");
  decl_codegen(d);
}

void stmt_codegen_first_pass(struct stmt *s) {
  if (!s) return;
  switch (s->kind) {
  	case STMT_BLOCK:
      stmt_codegen_first_pass(s->body);
  		break;
  	case STMT_DECL:
      first_pass(s->decl);
  		break;
    case STMT_EXPR:
      expr_gen_first_pass(s->expr->right);
      break;
    case STMT_IF_ELSE:
      expr_gen_first_pass(s->expr);
      stmt_codegen_first_pass(s->body);
      stmt_codegen_first_pass(s->else_body);
      break;
    case STMT_IF:
      expr_gen_first_pass(s->expr);
      stmt_codegen_first_pass(s->body);
      break;
    case STMT_FOR:
      stmt_codegen_first_pass(s->body);
      break;
    case STMT_PRINT:
      expr_gen_first_pass(s->expr);
      break;
    case STMT_RETURN:
      expr_gen_first_pass(s->expr);
      return;
  }
  stmt_codegen_first_pass(s->next);
}

// STRING_LITERAL DONE
// EXPR_ASSIGN DONE
// EXPR_ARG DONE
// EXPR_CALL DONE
// EXPR_ARR DO LATER
// EXPR_EQ DONE
// EXPR_NEQ DONE
void expr_gen_first_pass(struct expr* e) {
    // Handle string literals
    if(!e) return;
    if(e->kind == EXPR_STRING_LITERAL) {
      char* label = label_name(label_create());
      hash_table_insert(label_to_str, label, e->string_literal);

      printf("%s:\n", label);
      printf("\t .string \"%s\"\n", e->string_literal);

      e->reg = scratch_alloc();
    } else if(e->kind == EXPR_ASSIGN) {
      expr_gen_first_pass(e->right);
    } else if(e->kind == EXPR_EQ || e->kind == EXPR_NEQ) {
      expr_gen_first_pass(e->left);
      expr_gen_first_pass(e->right);
    } else if(e->kind == EXPR_ARG) {
      expr_gen_first_pass(e->left);
      expr_gen_first_pass(e->right);
    } else if(e->kind == EXPR_INTEGER_LITERAL) {
      expr_gen_first_pass(e->right);
    } 
}

void first_pass(struct decl *d) {
	if(!d) return;

  if(d->value) {
    if(d->value->kind == EXPR_CALL) {
      expr_gen_first_pass(d->value->right);
    } else {
      expr_gen_first_pass(d->value);
    }

    if(d->value->kind == EXPR_INTEGER_LITERAL) {
      printf("%s:\t.quad\t%d\n", d->symbol->name, d->value->literal_value);
    }
    if(d->value->kind == EXPR_CHAR_LITERAL) {
      printf("%s:\t.quad\t%c\n", d->symbol->name, d->value->literal_value);
    }
    if(d->value->kind == EXPR_BOOLEAN_LITERAL) {
      printf("%s:\t.quad\t%s\n", d->symbol->name, d->value->literal_value == 1 ? "true" : "false");
    }
  }

	if(d->code) {
      stmt_codegen_first_pass(d->code);
  }
	first_pass(d->next);
}
