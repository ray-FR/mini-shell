#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "command.h"

void exec_piped_cmds(struct command* cmd1, struct command* cmd2){
    pid_t pid1;
    pid_t pid2;
    int fdexec1;
    int fdexec2;

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
        if ((fdexec1 = dup2(0, 1)) < 0){
            perror("fdexec1");
            exit(1);
        }
        close(0);
        

    }

}