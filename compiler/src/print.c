#include <stdio.h>
#include <ctype.h>
#include "include/print.h"
#include "include/constants.h"

void decl_print(struct decl *d)
{
    if (!d)
        return;

    printf("%s: ", d->name);
    type_print(d->type);
    if (d->value)
    {
        printf(" = ");
        expr_print(d->value, 0, 0);
        printf(";");
    }
    if (d->code)
    {
        printf(" = {\n");
        stmt_print(d->code, 1, 1);
        printf("}");
    }
    printf("\n");
    decl_print(d->next);
}

void stmt_print(struct stmt *s, int indent, int indent_current)
{
    //printf("%d\n", s->kind);
    if (!s) {
        return;
    }

    if (indent_current)
        for (int i = 0; i < indent; i++)
            printf("    ");

    switch (s->kind)
    {
    case STMT_DECL:
        decl_print(s->decl);
        break;
    case STMT_EXPR:
        expr_print(s->expr, 0, 0);
        printf(";\n");
        break;
    case STMT_IF:
    case STMT_IF_ELSE:
        printf("if (");
        expr_print(s->expr, 0, 0);
        printf(") ");

        stmt_print(s->body, indent, 0);

        if (s->else_body)
        {
            for (int i = 0; i < indent; i++)
                printf("    ");
            printf("else ");

            stmt_print(s->else_body, indent, 0);
        
        }
        break;
    case STMT_FOR:
        printf("for (");
        expr_print(s->init_expr, 0, 0);
        printf("; ");
        expr_print(s->expr, 0, 0);
        printf("; ");
        expr_print(s->next_expr, 0, 0);
        printf(") ");
        stmt_print(s->body, indent, 0);
        break;
    case STMT_PRINT:
        printf("print ");
        expr_print(s->expr, 0, 0);
        printf(";\n");
        break;
    case STMT_RETURN:
        printf("return ");
        expr_print(s->expr, 0, 0);
        printf(";\n");
        break;
    case STMT_BLOCK:
        printf("{\n");
        stmt_print(s->body, indent + 1, 1);
        for (int i = 0; i < indent; i++)
            printf("    ");
        printf("}\n");
        break;
    default:
        break;
    }

    stmt_print(s->next, indent, 1);
}

void expr_print(struct expr *e, int parent_prec, int is_array_start)
{
    if (!e)
        return;
    int current_prec = get_precedence(e);

    // Handle leaf nodes (literals and names)
    if (!(e->left) && !(e->right))
    {
        switch (e->kind)
        {
        case EXPR_INTEGER_LITERAL:
            printf("%d", e->literal_value);
            break;
        case EXPR_BOOLEAN_LITERAL:
            printf(e->literal_value ? "true" : "false");
            break;
        case EXPR_CHAR_LITERAL:
            printf("\'%c\'", (char)e->literal_value);
            break;
        case EXPR_STRING_LITERAL:
            printf("\"");
            const char *str = e->string_literal;
            // Iterate through each character in the string
            while (*str)
            {
                switch (*str)
                {
                // Handle escape sequences
                case '\n':
                    printf("\\n");
                    break;
                case '\t':
                    printf("\\t");
                    break;
                case '\r':
                    printf("\\r");
                    break;
                case '\\':
                    printf("\\\\");
                    break;
                case '"':
                    printf("\\\"");
                    break;
                default:
                    // Print printable characters as-is, escape non-printable ones
                    if (isprint(*str))
                    {
                        putchar(*str);
                    }
                    else
                    {
                        printf("\\x%02x", (unsigned char)*str);
                    }
                }
                str++;
            }
            printf("\"");
            break;
        case EXPR_NAME:
            printf("%s", e->name);
            break;
        default:
            break;
        }
        return;
    }

    // Add parentheses if current expression has lower precedence than parent
    if (current_prec < parent_prec && e->kind != EXPR_NOT)
    {
        printf("(");
    }

    // Handle left child of binary expressions (except EXPR_NOT)
    if (e->left && e->kind != EXPR_NOT)
    {
        // Special handling for array expressions
        if (!is_array_start && e->kind == EXPR_ARR)
        {
            printf("{");
            is_array_start = 1;
        }
        if (e->left->kind == EXPR_ARR)
            printf("{");
        expr_print(e->left, current_prec, is_array_start);
    }

    // Print the operator or handle special cases
    switch (e->kind)
    {
    case EXPR_ADD:
        printf(" + ");
        break;
    case EXPR_SUB:
        printf(" - ");
        break;
    case EXPR_MUL:
        printf(" * ");
        break;
    case EXPR_DIV:
        printf(" / ");
        break;
    case EXPR_ASSIGN:
        printf(" = ");
        break;
    case EXPR_LT:
        printf(" < ");
        break;
    case EXPR_LTE:
        printf(" <= ");
        break;
    case EXPR_GT:
        printf(" > ");
        break;
    case EXPR_GTE:
        printf(" >= ");
        break;
    case EXPR_MOD:
        printf(" %% ");
        break;
    case EXPR_AND:
        printf(" && ");
        break;
    case EXPR_OR:
        printf(" || ");
        break;
    case EXPR_NOT:
    printf("!");
    
    // Determine if parentheses are needed around the inner expression
    int left_prec = get_precedence(e->left);
    
    // Add parentheses if the left expression has lower precedence
    // OR if it's a post-increment or post-decrement operation (which need grouping)
    if (left_prec < current_prec || e->left->kind == EXPR_INCR || e->left->kind == EXPR_DECR)
    {
        printf("(");
        expr_print(e->left, 0, is_array_start);
        printf(")");
    }
    else
    {
        expr_print(e->left, 0, is_array_start);
    }
    break;


    case EXPR_EXP:
        printf("^");
        break;
    case EXPR_EQ:
        printf(" == ");
        break;
    case EXPR_NEQ:
        printf(" != ");
        break;
    case EXPR_INCR:
        printf("++");
        break;
    case EXPR_DECR:
        printf("--");
        break;
    case EXPR_CALL:
        printf("(");
        expr_print(e->right, 0, is_array_start);
        printf(")");
        break;
    case EXPR_SUBSCRIPT:
        printf("[");
        expr_print(e->right, 0, is_array_start);
        printf("]");
        break;
    case EXPR_ARG:
        if (e->right)
        {
            printf(", ");
            expr_print(e->right, 0, is_array_start);
        }
        break;
    case EXPR_ARR:
        if (e->right)
        {
            printf(", ");
            expr_print(e->right, 0, is_array_start);
        }
        else
        {
            printf("}");
        }
        break;
    default:
        break;
    }

    // Handle right child of binary expressions
    if (e->right && e->kind != EXPR_CALL && e->kind != EXPR_SUBSCRIPT && e->kind != EXPR_ARG && e->kind != EXPR_ARR)
    {
        expr_print(e->right, current_prec + 1, is_array_start);
    }

    // Close parentheses if needed
    if (current_prec < parent_prec && e->kind != EXPR_NOT)
    {
        printf(")");
    }
}

// Helper function to determine operator precedence
int get_precedence(struct expr *e)
{
    switch (e->kind)
    {
    case EXPR_ASSIGN:
        return PREC_ASSIGN;
    case EXPR_ADD:
    case EXPR_SUB:
        return PREC_ADD;
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_MOD:
        return PREC_MULTIPLY;
    case EXPR_CALL:
        return PREC_CALL;
    case EXPR_NOT:
        return PREC_LOG_NOT;
    case EXPR_SUBSCRIPT:
        return PREC_SUBSCRIPT;
    case EXPR_EXP:
        return PREC_EXPONENTIAL;
    case EXPR_INCR:
    case EXPR_DECR:
        return PREC_INCR;
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    case EXPR_EQ:
    case EXPR_NEQ:
        return PREC_COMPARE;
    case EXPR_AND:
        return PREC_LOG_AND;
    case EXPR_OR:
        return PREC_LOG_OR;
    default:
        return PREC_SUBSCRIPT; // Default to highest precedence
    }
}

void param_list_print(struct param_list *p)
{
    if (!p)
        return;
    printf("%s: ", p->name);
    type_print(p->type);
    if (p->next)
    {
        printf(", ");
        param_list_print(p->next);
    }
}

void type_print(struct type *t)
{
    if (!t)
        return;
    switch (t->kind)
    {
    case TYPE_VOID:
        printf("void");
        break;
    case TYPE_BOOLEAN:
        printf("boolean");
        break;
    case TYPE_CHARACTER:
        printf("char");
        break;
    case TYPE_INTEGER:
        printf("integer");
        break;
    case TYPE_STRING:
        printf("string");
        break;
    case TYPE_ARRAY:
        printf("array ");
        if (t->array_size > 0)
            printf("[%d] ", t->array_size);
        else
            printf("[] ");
        type_print(t->subtype);
        break;
    case TYPE_FUNCTION:
        printf("function ");
        type_print(t->subtype);
        printf(" (");
        param_list_print(t->params);
        printf(")");
        break;
    default:
        break;
    }
}

void symbol_print(struct symbol *s) {
  printf("name: %s, kind: %s\n", s->name, SCOPE_LOOKUP[s->kind]);
}
