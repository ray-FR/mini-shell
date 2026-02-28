#include "command.h"
#include "envvar.h"

int builtin_export(struct command* cmd, struct envVar_t* EV){
    char varName[80];
    strcpy(varName, "$");
    if (cmd->argc < 3){
        printf("usage: export VARIABLE_NAME = <value>\n");
        return 1;
    }
    else if (cmd->argc == 3){
        if(cmd->argv[1][0] == '$') strcpy(varName, cmd->argv[1]);
        else strcat(varName, cmd->argv[1]);
        EV->nbVar++;
        strcpy(EV->vars[EV->nbVar - 1]->ENVVAR, varName);
        strcpy(EV->vars[EV->nbVar - 1]->ENVVAR, cmd->argv[2]);
        return 0;

    }
    else if(cmd->argc > 4 || (strcmp(cmd->argv[2], "=") != 0)){
        printf("export: too many arguments\n");
        return 1;
    }
    if(cmd->argc == 4){
        if((cmd->argv[1])[0] == '$') strcpy(varName, cmd->argv[1]);
        else strcat(varName, cmd->argv[1]);
        EV->nbVar++;
        strcpy(EV->vars[EV->nbVar - 1]->ENVVAR, varName);
        strcpy(EV->vars[EV->nbVar - 1]->value, cmd->argv[3]);
        return 0;

    }
    
    printf("builtin_export: error\n");
    return 1;

}