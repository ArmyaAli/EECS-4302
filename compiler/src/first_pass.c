#include "include/codegen.h"
#include "include/codegen_helper.h"

extern struct hash_table* label_to_str;

extern char* asm_output;
extern int asm_output_offset;

extern frame_t stack[];
extern int sp;

int extern_label = 0;


void first_pass(struct decl *d) {
	if(!d) return;

  if(d->value) {
    if(d->value->kind == EXPR_CALL) {
      expr_codegen_first_pass(d->value->right);
    } else {
      expr_codegen_first_pass(d->value);
    }

    if(d->value->kind == EXPR_INTEGER_LITERAL) {
      printf("%s:\t.quad\t%d\n", d->symbol->name, d->value->literal_value);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s:\t.quad\t%d\n", d->symbol->name, d->value->literal_value);
    }
    if(d->value->kind == EXPR_CHAR_LITERAL) {
      printf("%s:\t.quad\t%c\n", d->symbol->name, d->value->literal_value);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s:\t.quad\t%c\n", d->symbol->name, d->value->literal_value);
    }
    if(d->value->kind == EXPR_BOOLEAN_LITERAL) {
      printf("%s:\t.quad\t%s\n", d->symbol->name, d->value->literal_value == 1 ? "true" : "false");
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s:\t.quad\t%s\n", d->symbol->name, d->value->literal_value == 1 ? "true" : "false");
    }
    if(d->value->kind == EXPR_STRING_LITERAL) {
      printf("%s:\t.string\t\"%s\"\n", d->symbol->name, d->value->string_literal);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s:\t.string\t\"%s\"\n", d->symbol->name, d->value->string_literal);
    }
  }

  // Here we will generate function prologue
	if(d->code) {
    printf(".global %s\n", d->name);
    asm_output_offset += sprintf(asm_output + asm_output_offset, ".global %s\n", d->name);
    stmt_codegen_first_pass(d->code);
  }
	first_pass(d->next);
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
      expr_codegen_first_pass(s->expr->right);
      break;
    case STMT_IF_ELSE:
      expr_codegen_first_pass(s->expr);
      stmt_codegen_first_pass(s->body);
      stmt_codegen_first_pass(s->else_body);
      break;
    case STMT_IF:
      expr_codegen_first_pass(s->expr);
      stmt_codegen_first_pass(s->body);
      break;
    case STMT_FOR:
      stmt_codegen_first_pass(s->body);
      break;
    case STMT_PRINT:
      if(!extern_label) {
        printf(".extern printf\n");
        asm_output_offset += sprintf(asm_output + asm_output_offset, "extern printf\n");
      }
      extern_label = 1;
      expr_codegen_first_pass(s->expr);
      break;
    case STMT_RETURN:
      expr_codegen_first_pass(s->expr);
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
void expr_codegen_first_pass(struct expr* e) {
    // Handle string literals
    if(!e) return;
    if(e->kind == EXPR_STRING_LITERAL) {
      char* label = label_name(label_create());
      hash_table_insert(label_to_str, label, e->string_literal);
      e->reg = scratch_alloc();
    } else if(e->kind == EXPR_ASSIGN) {
      expr_codegen_first_pass(e->right);
    } else if(e->kind == EXPR_EQ || e->kind == EXPR_NEQ) {
      expr_codegen_first_pass(e->left);
      expr_codegen_first_pass(e->right);
    } else if(e->kind == EXPR_ARG) {
      expr_codegen_first_pass(e->left);
      expr_codegen_first_pass(e->right);
    } else if(e->kind == EXPR_INTEGER_LITERAL) {
      expr_codegen_first_pass(e->right);
    } 
}
