#include "../include/type.h"
#include <stdlib.h>

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params) {
  struct type *t = malloc(sizeof(*t));
  t->kind = kind;
  t->subtype = subtype;
  t->params = params;
  return t;
}
