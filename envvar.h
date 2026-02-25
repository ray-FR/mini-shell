#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct EVAR {
    char ENVVAR[80];
    char value[512];
};

struct envVar_t {
    int nbVar;
    struct EVAR** vars;
};


struct envVar_t * initEnvVar();
void freeEnvVar(struct envVar_t* EV);