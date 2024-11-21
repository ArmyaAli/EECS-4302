#define _CRT_SECURE_NO_DEPRECATE

#include "include/global.h"
#include "include/messages.h"
#include "include/pipeline.h"

int main(int argc, char *argv[]) {
    const char *option = argv[1];
    const char *filename = argv[2];

    if (argc < 3) {
        fprintf(stderr, USAGEMSG_SCANNER,      argv[0]);
        fprintf(stderr, USAGEMSG_PARSER,       argv[0]);
        fprintf(stderr, USAGEMSG_TYPECHECKER,  argv[0]);
        fprintf(stderr, USAGEMSG_NAMERESOLVER, argv[0]);
        exit(1);
    }

    if (strcmp(option, "-scan") == 0) run_scan(filename); 
    else if (strcmp(option, "-parse") == 0) run_parser(filename); 
    else if (strcmp(option, "-resolve") == 0) run_resolve(filename); 
    else if (strcmp(option, "-typecheck") == 0) run_typecheck(filename); 
    else {
        fprintf(stderr, "Unknown option: %s\n", option);
        exit(1);
    }

    return 0;
}

