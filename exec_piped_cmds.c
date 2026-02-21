#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "command.h"

void exec_piped_cmds(struct command* cmd1, struct command* cmd2){
    pid_t pid1;
    pid_t pid2;

    int fd[2] = {1, 0};
    if (pipe(fd) < 0){
        perror("pipe");
        exit(1);
    }
    if ((pid1 = fork()) < 0){
        perror("fork1");
        exit(1);
    }
    if (pid1 == 0){
        
    }

}