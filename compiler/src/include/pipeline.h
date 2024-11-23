#ifndef PIPELINE_H
#define PIPELINE_H

#include "global.h"
#include "messages.h"
#include "constants.h"
#include "print.h"
#include "stack.h"
#include "resolve.h"

void run_scan(const char* filename); // Line number of the last scanned text.
void run_parser(const char *);
void run_resolve(const char *);
void run_typecheck(const char *);
void run_print(struct stmt* ast);

#endif
