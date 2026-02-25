#include "envvar.h"

struct envVar_t * initEnvVar(){
    struct envVar_t* EV = NULL;
    EV = malloc(sizeof(*EV));
    EV->nbVar = 0;
}