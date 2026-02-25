#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "command.h"

struct EVAR {
    char ENVVAR[80];
    char value[512];
};

struct envVar_t {
    int nbVar;
    struct EVAR** vars;
};


struct envVar_t * initEnvVar();
void replaceEnvVar(struct envVar_t* EV, struct command* cmd);
void freeEnvVar(struct envVar_t* EV);