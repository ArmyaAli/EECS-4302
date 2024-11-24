#ifndef PIPELINE_H
#define PIPELINE_H

void run_scan(const char* filename); // Line number of the last scanned text.
void run_parser(const char *);
void run_resolve(const char *);
void run_typecheck(const char *);

#endif
