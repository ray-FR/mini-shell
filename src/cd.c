#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "command.h"


int builtin_cd(struct command* cmd){

    if(chdir(cmd->argv[(cmd->argc)-1]) < 0){
        perror("chdir");
        return 1;
    }
    return 0;
}