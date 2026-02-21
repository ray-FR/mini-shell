#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "command.h"

void exec_piped_cmds(struct command* cmd1, struct command* cmd2){
    int fd[2] = {1, 0};
    if (pipe(fd) < 0){
        perror("pipe");
        exit(1);
    }


}