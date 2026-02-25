#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct EVAR {
    char* ENVVAR;
    char* value;
};

struct envVar_t {
    int nbVar;
    struct EVAR* vars;
};