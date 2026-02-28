#include <fcntl.h>
#include "command.h"



void builtin_redirect(struct command* cmd, char* fileName, int typeOfRedirection){
    int fd;
    pid_t pid;
    
    if ((fd = open(fileName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR)) < 0){
        perror("fd");
        exit(1);
    }
    
    
    if ((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        if (ftruncate(fd, 0) == 0) 
            lseek(fd, 0, SEEK_SET); // reposition to start for subsequent writes
        
        dup2(fd, 1);
        close(fd);
        execvp(cmd->argv[0], &cmd->argv[0]);
    }
    else{
        close(fd);
        waitpid(pid, NULL, 0);
    }
}


