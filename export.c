#include "command.h"
#include "envvar.h"

int builtin_export(struct command* cmd, struct envVar_t* EV){
    if(cmd->argc > 2){
        printf("export: too many arguments\n");
        return 1;
    }
    else if (cmd->argc < 2){
        printf("usage: export VARIABLE_NAME = <value>\n");
        return 1;
    }

    
}