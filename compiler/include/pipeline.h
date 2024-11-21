#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdlib.h>
#include <stdio.h>
#include "../include/token.h"
#include "../include/messages.h"
#include "../include/constants.h"
#include "../include/print.h"
#include "../include/stack.h"
#include "../include/resolve.h"
#include "../include/global.h"

void run_parser(const char *);
void run_resolve(const char *);
void run_typecheck(const char *);
void run_print(struct stmt* ast);

#endif
