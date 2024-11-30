#include "include/decl.h"
#include <stdlib.h>

struct decl *decl_create(char *name, struct type *type, struct expr *value,
                         struct stmt *code, struct decl *next) {
  struct decl *d = malloc(sizeof(*d));
  d->name = name;
  d->type = type;
  d->value = value;
   // expr_copy(value);
  d->code = code;
  d->next = next;
  return d;
}

struct expr* expr_copy(struct expr* original) {
    if (original == NULL) return NULL;

    // Allocate memory for the new expression
    struct expr *copy = malloc(sizeof(struct expr));
    if (!copy) return NULL;  // Check for allocation failure

    // Copy the kind
    copy->kind = original->kind;

    // Recursively copy the left and right sub-expressions
    copy->left = expr_copy(original->left);
    copy->right = expr_copy(original->right);

    // Deep copy the name if it exists
    if (original->name) {
        copy->name = strdup(original->name);  // strdup allocates and copies the string
        if (!copy->name) {
            // If strdup fails, clean up and return NULL
            free(copy->left);
            free(copy->right);
            free(copy);
            return NULL;
        }
    } else {
        copy->name = NULL;
    }

    // Copy the literal_value
    copy->literal_value = original->literal_value;

    // Deep copy the string_literal if it exists
    if (original->string_literal) {
        copy->string_literal = strdup(original->string_literal);
        if (!copy->string_literal) {
            // If strdup fails, clean up and return NULL
            free(copy->name);
            free(copy->left);
            free(copy->right);
            free(copy);
            return NULL;
        }
    } else {
        copy->string_literal = NULL;
    }

    // Copy the symbol pointer (assuming shallow copy, as deep copy depends on the symbol structure)
    copy->symbol = original->symbol;

    return copy;
}
