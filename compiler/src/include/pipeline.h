#ifndef PIPELINE_H
#define PIPELINE_H

#include "messages.h"
#include "constants.h"
#include "print.h"
#include "stack.h"
#include "resolve.h"
#include "global.h"

void run_parser(const char *);
void run_resolve(const char *);
void run_typecheck(const char *);
void run_print(struct stmt* ast);

#endif
