#include "command.h"
#include "envvar.h"

int builtin_export(struct command* cmd, struct envVar_t* EV){
    char* varName = "$";
    if(cmd->argc > 4){
        printf("export: too many arguments\n");
        return 1;
    }
    else if (cmd->argc < 2){
        printf("usage: export VARIABLE_NAME = <value>\n");
        return 1;
    }
    if(cmd->argc == 4){
        if(cmd->argv[1][0] != '$') strcat(varName, cmd->argv[1]);
        else varName = strdup(cmd->argv[1]);
        EV->nbVar++;
        strcpy(EV->vars[EV->nbVar - 1]->ENVVAR, varName);
        strcpy(EV->vars[EV->nbVar - 1]->ENVVAR, cmd->argv[4]);
        return 0;

    }
    
    if(cmd->argv[1][0] != '$') strcat(varName, cmd->argv[1]);
    else varName = strdup(cmd->argv[1]);
    EV->nbVar++;
    strcpy(EV->vars[EV->nbVar - 1]->ENVVAR, varName);
    strcpy(EV->vars[EV->nbVar - 1]->ENVVAR, cmd->argv[2]);


    return 0;

}