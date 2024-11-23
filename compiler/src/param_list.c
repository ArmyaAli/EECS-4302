#include "include/param_list.h"
#include <stdlib.h>

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {
  struct param_list * param_l = malloc(sizeof(*param_l));
  param_l->name = name;
  param_l->type = type;
  param_l->next = next;
  return param_l;
}

int param_list_size(struct param_list *list) {
  struct param_list* cursor = list;
  int size = 0;
  while(cursor != NULL) {
    ++size;
    cursor = cursor->next;
  }
  return size;
}
