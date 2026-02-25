#include "envvar.h"

struct envVar_t * initEnvVar(){
    struct envVar_t* EV = NULL;
    EV = malloc(sizeof(*EV));
    EV->nbVar = 2;
    EV->vars = malloc(EV->nbVar * sizeof(*EV->vars));
    for(int i =0;i<2;i++)
        EV->vars[i] = malloc(sizeof(*EV->vars[i]));

    strcpy(EV->vars[0]->ENVVAR, "$PWD");
    if (getcwd(EV->vars[0]->value, 512) == NULL){perror("getcwd"); exit(1);}

    strcpy(EV->vars[1]->ENVVAR, "$USER");
    char* ec = getlogin();
    (strcpy(EV->vars[1]->value, ec));
    //free(ec);
    return EV;
}

void freeEnvVar(struct envVar_t* EV){
    for (int i = 0; i<EV->nbVar; i++ ){
        free(EV->vars[i]);
    }
    free(EV->vars);
    free(EV);
}