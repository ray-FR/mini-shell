#include <fcntl.h>
#include "command.h"

void builtin_redirect(struct command* cmd){
    int fd;
    pid_t pid;
    printf("%s\n", cmd->argv[3]);
    if ((fd = open(cmd->argv[3], O_WRONLY)) < 0){
        perror("fd");
        exit(1);
    }
    if ((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        dup2(fd, 1);
        close(fd);
        execvp(cmd->argv[0], &cmd->argv[0]);
    }
    else{
        close(fd);
        waitpid(pid, NULL, 0);
        printf("woo the bullshit worked\n");
    }
}


