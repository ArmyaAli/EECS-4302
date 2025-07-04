#include "include/codegen.h"
#include "include/codegen_helper.h"
#include "include/constants.h"
#include "include/typecheck.h"
#include <stdio.h>

extern char* asm_output;
extern int asm_output_offset;


extern frame_t stack[];
extern int sp;
extern struct hash_table* label_to_str;

void second_pass(struct decl *d) {
	if(!d) return;

  if(d->value) {
    if(d->value->kind == EXPR_CALL) {
      expr_codegen_second_pass(d->value->right);
    } else {
      expr_codegen_second_pass(d->value);
    }
    if(d->symbol && d->symbol->kind != SYMBOL_GLOBAL) {
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %%%s, %s\n", scratch_name(d->value->reg), symbol_codegen(d->symbol));
    }
  }
 
  // This should generate the function prologue
  // First setup stack base pointer
 	if(d->code) {
     asm_output_offset += sprintf(asm_output + asm_output_offset, ".global %s\n", d->name);

     asm_output_offset += sprintf(asm_output + asm_output_offset, "%s:\n", d->name);
     asm_output_offset += sprintf(asm_output + asm_output_offset, "\tPUSHQ %%%s \n", SCRATCH_LOOKUP[0]);
     asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %%%s, %%%s\n", SCRATCH_LOOKUP[0], SCRATCH_LOOKUP[1]);

     stmt_codegen_second_pass(d->code);
  }

 	second_pass(d->next);
}

void expr_codegen_second_pass(struct expr *e) {
  if (!e) return;

  switch (e->kind) {
    case EXPR_NAME:

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
          asm_output_offset += sprintf(asm_output + asm_output_offset, "\tLEAQ %s, %%%s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
        }
        else {
          asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %s, %%%s\n", symbol_codegen(e->symbol), scratch_name(e->reg));
        }
      }
      break;
    case EXPR_INTEGER_LITERAL:
      e->reg = scratch_alloc();
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ $%d, %%%s\n", e->literal_value, scratch_name(e->reg));
      break;
    case EXPR_STRING_LITERAL:
      //e->reg = scratch_alloc();
      break;
    case EXPR_CHAR_LITERAL:
      e->reg = scratch_alloc();
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ $%c, %%%s\n", e->literal_value, scratch_name(e->reg));
      break;
    case EXPR_BOOLEAN_LITERAL:
      e->reg = scratch_alloc();
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ $%s, %%%s\n", e->literal_value == 0 ? "False" : "True", scratch_name(e->reg));
      break;
    case EXPR_ADD:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tADDQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
      e->reg = e->right->reg;
      scratch_free(e->left->reg);
      break;
    case EXPR_SUB:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tSUBQ %%%s, %%%s\n", scratch_name(e->right->reg), scratch_name(e->left->reg));
      e->reg = e->left->reg;
      scratch_free(e->right->reg);
      break;
    case EXPR_MUL:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);

      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %%%s, %%rax\n", scratch_name(e->left->reg));
      sprintf(asm_output + asm_output_offset, "\tIMULQ %%%s\n", scratch_name(e->right->reg));
      e->reg = scratch_alloc();
      sprintf(asm_output + asm_output_offset, "\tMOVQ %%rax, %%%s\n", scratch_name(e->reg));

      scratch_free(e->left->reg);
      scratch_free(e->right->reg);
      break;

    case EXPR_DIV:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);

      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %%%s, %%rax\n", scratch_name(e->left->reg));
      sprintf(asm_output + asm_output_offset, "\tCQO\n");
      sprintf(asm_output + asm_output_offset, "\tIDIVQ %%%s\n", scratch_name(e->right->reg));

      e->reg = scratch_alloc();
      sprintf(asm_output + asm_output_offset, "\tMOVQ %%rax, %%%s\n", scratch_name(e->reg));

      scratch_free(e->left->reg);
      scratch_free(e->right->reg);
      break;

    case EXPR_MOD:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);

      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %%%s, %%rax\n", scratch_name(e->left->reg));
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tCQO\n");
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tIDIVQ %%rax\n");

      e->reg = scratch_alloc();
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %%rdx, %%%s\n", scratch_name(e->reg));

      // Free the used registers
      scratch_free(e->left->reg);
      scratch_free(e->right->reg);
      break;

    case EXPR_ASSIGN:
      expr_codegen_second_pass(e->right);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tMOVQ %%%s, %s\n", scratch_name(e->right->reg), symbol_codegen(e->left->symbol));
      break;
    case EXPR_CALL:
      break;
    case EXPR_ARG:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);
      break;
    case EXPR_SUBSCRIPT:
      break;
    
    case EXPR_AND:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);

      if (e->left->kind == EXPR_NOT) {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tANDQ %%%s, %%%s\n", scratch_name(e->left->left->reg), scratch_name(e->right->reg));
        e->reg = e->right->left->reg;
        scratch_free(e->left->reg);
      }
      else if (e->right->kind == EXPR_NOT) {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tANDQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->left->reg));
        e->reg = e->right->left->reg;
        scratch_free(e->left->reg);
      }
      else if (e->left->kind == EXPR_NOT && e->right->kind == EXPR_NOT) {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tANDQ %%%s, %%%s\n", scratch_name(e->left->left->reg), scratch_name(e->right->left->reg));
        e->reg = e->right->left->reg;
        scratch_free(e->left->reg);
      }
      else {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tANDQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
        e->reg = e->right->reg;
        scratch_free(e->left->reg);
      }
      break;
    case EXPR_OR:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);

      if (e->left->kind == EXPR_NOT) {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tORQ %%%s, %%%s\n", scratch_name(e->left->left->reg), scratch_name(e->right->reg));
        e->reg = e->right->left->reg;
        scratch_free(e->left->reg);
      }
      else if (e->right->kind == EXPR_NOT) {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tORQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->left->reg));
        e->reg = e->right->left->reg;
        scratch_free(e->left->reg);
      }
      else if (e->left->kind == EXPR_NOT && e->right->kind == EXPR_NOT) {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tORQ %%%s, %%%s\n", scratch_name(e->left->left->reg), scratch_name(e->right->left->reg));
        e->reg = e->right->left->reg;
        scratch_free(e->left->reg);
      }
      else {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tORQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
        e->reg = e->right->reg;
        scratch_free(e->left->reg);
      }
      break;
    case EXPR_NOT:
      expr_codegen_second_pass(e->left);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tNOTQ %%%s\n", scratch_name(e->left->reg));
      break;
    case EXPR_EXP:break;

    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
      expr_codegen_second_pass(e->left);
      expr_codegen_second_pass(e->right);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tCMPQ %%%s, %%%s\n", scratch_name(e->left->reg), scratch_name(e->right->reg));
      break;

    case EXPR_EQ:break;
    case EXPR_NEQ:break;
    case EXPR_INCR:
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tINCQ %%%s\n", scratch_name(e->left->reg));
      break;
    case EXPR_DECR:
      asm_output_offset += sprintf("\tDECQ %%%s\n", scratch_name(e->left->reg));
      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tDECQ %%%s\n", scratch_name(e->left->reg));
      break;

    case EXPR_ARR:break;
  }
}

void stmt_codegen_second_pass(struct stmt *s) {
 if (!s) return;
 switch (s->kind) {
 	case STMT_BLOCK:
     stmt_codegen_second_pass(s->body);
 		break;
 	case STMT_DECL:
     second_pass(s->decl);
 		break;
   case STMT_EXPR:
     expr_codegen_second_pass(s->expr);
     break;
   case STMT_IF_ELSE:
     expr_codegen_second_pass(s->expr);

     if (s->expr->kind == EXPR_GT) {
      char* label_if = label_name(label_create());

      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJLE %s\n", label_if);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

      stmt_codegen_second_pass(s->body);

      char* label_if_else = label_name(label_create());
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if_else);

      stmt_codegen_second_pass(s->else_body);
     }
     else if (s->expr->kind == EXPR_LT) {
      char* label_if = label_name(label_create());

      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJGE %s\n", label_if);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

      stmt_codegen_second_pass(s->body);

      char* label_if_else = label_name(label_create());
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if_else);

      stmt_codegen_second_pass(s->else_body);
     }
      else if (s->expr->kind == EXPR_GTE) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJL %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);

        char* label_if_else = label_name(label_create());
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if_else);

        stmt_codegen_second_pass(s->else_body);
     }
      else if (s->expr->kind == EXPR_LTE) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJG %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);

        char* label_if_else = label_name(label_create());
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if_else);

     }
      else if (s->expr->kind == EXPR_EQ) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJNE %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);

        char* label_if_else = label_name(label_create());
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if_else);

        stmt_codegen_second_pass(s->else_body);
     }
      else if (s->expr->kind == EXPR_NEQ) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJE %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);

        char* label_if_else = label_name(label_create());
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if_else);

        stmt_codegen_second_pass(s->else_body);
     }
     break;
   case STMT_IF:
     expr_codegen_second_pass(s->expr);

     if (s->expr->kind == EXPR_GT) {
      char* label_if = label_name(label_create());

      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJLE %s\n", label_if);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

      stmt_codegen_second_pass(s->body);
     }
     else if (s->expr->kind == EXPR_LT) {
      char* label_if = label_name(label_create());

      asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJGE %s\n", label_if);
      asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

      stmt_codegen_second_pass(s->body);
     }
      else if (s->expr->kind == EXPR_LTE) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJG %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);
     }
      else if (s->expr->kind == EXPR_GTE) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJL %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);
     }
      else if (s->expr->kind == EXPR_EQ) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJNE %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);
     }
      else if (s->expr->kind == EXPR_NEQ) {
        char* label_if = label_name(label_create());

        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJE %s\n", label_if);
        asm_output_offset += sprintf(asm_output + asm_output_offset, "%s: \n", label_if);

        stmt_codegen_second_pass(s->body);
     }
     break;
   case STMT_FOR:
      /* This alternative will print additional instructions that doesn't match the instructions as defined in pg. 158 of the textbook */
       expr_codegen_second_pass(s->init_expr);
       asm_output_offset += sprintf(asm_output + asm_output_offset, "loop:\t");
       expr_codegen_second_pass(s->next_expr);
       expr_codegen_second_pass(s->expr);

      if (!s->expr) {
        asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJMP  loop\n");
      } else {
          if (s->expr->kind == EXPR_LT) {
            asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJL  loop\n");
          } else if (s->expr->kind == EXPR_LTE) {
            asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJLE  loop\n");
          } else if (s->expr->kind == EXPR_GT) {
            asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJG  loop\n");
          } else if (s->expr->kind == EXPR_GTE) {
            asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJGE  loop\n");
          } else if (s->expr->kind == EXPR_EQ) {
            asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJE  loop\n");
          } else if (s->expr->kind == EXPR_NEQ) {
            asm_output_offset += sprintf(asm_output + asm_output_offset, "\tJNE  loop\n");
          }   
        }
     break;
   case STMT_PRINT:
     /* Use library.c instructions to print instead of generating code for the arg, list passed in print */
     struct expr* current = s->expr;
     while (current != NULL) {
      const char* name = current->string_literal;
      struct expr* name_left = current->left;

      char* label = NULL;
      if(name_left) {
        char* key;
        char* val;
        hash_table_firstkey(label_to_str);
        while(hash_table_nextkey(label_to_str, &key, (void*)(&val))) {
          if (strcmp(name_left->string_literal, val) == 0) {
            label = strdup(key);
            const char* reg = scratch_name(scratch_alloc());
            if(reg == NULL) {
              printf("ERROR: too many allocations\n");
              return;
            }
            asm_output_offset += sprintf(asm_output + asm_output_offset, "LEAQ %s(%%rip), %%%s\n", label, reg);
          }
        }
      } else {
        char* key;
        char* val;
        hash_table_firstkey(label_to_str);
        while(hash_table_nextkey(label_to_str, &key, (void*)(&val))) {
          if (strcmp(name, val) == 0) {
            label = strdup(key);
            const char* reg = scratch_name(scratch_alloc());
            if(reg == NULL) {
              printf("ERROR: too many allocations\n");
              return;
            }
            asm_output_offset += sprintf(asm_output + asm_output_offset, "LEAQ %s(%%rip), %%%s\n", label, reg);
          }
        }
      } 

      struct type* t = expr_typecheck(current);
      if(t) {
        if (t->kind == TYPE_INTEGER) { 
          asm_output_offset += sprintf(asm_output + asm_output_offset, "CALL print_integer\n"); 
        }
        if (t->kind == TYPE_BOOLEAN) { 
          asm_output_offset += sprintf(asm_output + asm_output_offset, "CALL print_boolean\n"); 
        }
        if (t->kind == TYPE_CHARACTER) { 
          asm_output_offset += sprintf(asm_output + asm_output_offset, "CALL print_character\n"); 
        }
        if (t->kind == TYPE_STRING) { 
          asm_output_offset += sprintf(asm_output + asm_output_offset, "CALL print_string\n"); 
        }
      }
      current = current->right;
     }
     break;
   case STMT_RETURN:
     expr_codegen_second_pass(s->expr);
     asm_output_offset += sprintf(asm_output + asm_output_offset, "RET\n"); 
     return;
 }
 stmt_codegen_second_pass(s->next);
}
