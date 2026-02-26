#include <stdio.h>
#include <stdlib.h>
#include "command.h"

int builtin_exit(struct command* cmd){
    (void) cmd,

    exit(0);
    return 0;
}