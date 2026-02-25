#include <stdio.h>
#include <stdlib.h>
#include "command.h"

int builtin_exit(struct command* cmd){
    free_cmd(cmd);

    exit(0);
    return 0;
}