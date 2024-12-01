#include "include/codegen.h"
#include "include/codegen_helper.h"
#include <stdio.h>

void expr_codegen(struct expr *e) {
  if (!e) return;

  switch (e->kind) {
    case EXPR_NAME:
      printf("\tCODE_GEN_EXPRNAME\n");
      e->reg = scratch_alloc();
      printf("MOVQ %%%s, %%%s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
      break;
    case EXPR_INTEGER_LITERAL:
      printf("\tCODE_GEN_INT\n");
      e->reg = scratch_alloc();
      printf("MOVQ $%d, %%%s\n", e->literal_value, scratch_name(e->reg));
      break;
    case EXPR_STRING_LITERAL:
      printf("\tCODE_GEN_STRING\n");
      e->reg = scratch_alloc();
      printf("MOVQ $%s, %%%s\n", e->string_literal, scratch_name(e->reg));
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
      printf("IMUL %%%s \n", scratch_name(e->left->reg));
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
      expr_codegen(e->left);
      // printf("left %s\n", e->left->name);
      expr_codegen(e->right);
      // printf("right %s\n", e->right->left->name);
      // printf("right %s\n", e->right->right->literal_value);
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
  printf("\tSTMT_CODEGEN\n");
  if (!s) return;
	switch (s->kind) {
		case STMT_BLOCK:
      stmt_codegen(s->body);
			break;
		case STMT_DECL:
      decl_codegen(s->decl);
			break;
    case STMT_EXPR:
      expr_codegen(s->expr);
      break;
    case STMT_IF_ELSE:
      break;
    case STMT_IF:
      break;
    case STMT_FOR:
      break;
    case STMT_PRINT:
      break;
    case STMT_RETURN:
      return;
	}
  stmt_codegen(s->next);
}

void decl_codegen(struct decl *d) {
  printf("\tDECL_CODEGEN\n");
	if(!d) return;

  if(d->value) {
    if(d->value->kind == EXPR_CALL) {
      expr_codegen(d->value->left);
    } else {
      expr_codegen(d->value);
    }
    printf("MOVQ %%%s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
  }
  
	if(d->code) {
      stmt_codegen(d->code);
    }
	decl_codegen(d->next);
}