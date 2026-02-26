#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "command.h"

void exec_piped_cmds(struct command* cmd1, struct command* cmd2){
    pid_t pid1;
    pid_t pid2;

    int fd[2];
    if (pipe(fd) < 0){
        perror("pipe");
        exit(1);
    }
    if ((pid1 = fork()) < 0){
        perror("fork1");
        exit(1);
    }
    if (pid1 == 0){
        if ((dup2(fd[1], 1)) < 0){
            perror("fdexec1");
            exit(1);
        }
        close(fd[0]);
        signal(SIGINT, SIG_DFL);
        execvp(cmd1->argv[0], &(cmd1->argv[0]));


    }
    if ((pid2 = fork()) < 0){
        perror("pid2");
        exit(1);
    }

    if (pid2 == 0){
        if ((dup2(fd[0], 0)) < 0){
            perror("fdexec2");
            exit(1);
        }
        close(fd[1]);
        signal(SIGINT, SIG_DFL);
        execvp(cmd2->argv[0], &(cmd2->argv[0]));
    }
    else if (pid2 > 0) {
        close(fd[1]);
        close(fd[0]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        signal(SIGINT, SIG_IGN);
    }



}