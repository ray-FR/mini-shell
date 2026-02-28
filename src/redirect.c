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
        if (typeOfRedirection == 0){
            if (lseek(fd, 0, SEEK_END) < 0){
                perror("lseek");
                exit(1);
            }
        }
        
        else if (typeOfRedirection == 1){    
            if (ftruncate(fd, 0) == 0) 
                lseek(fd, 0, SEEK_SET); 
            else {
                perror("ftruncate");
                exit(1);
            }
        }    
        if (dup2(fd, 1) < 0) {
            perror("dup2");
            exit(1);
        }
        if (close(fd) < 0){
            perror("close");
            exit(1);
        }

        signal(SIGINT, SIG_DFL);
        execvp(cmd->argv[0], &cmd->argv[0]);
        perror("execvp");
        exit(1);
    }
    else{
        close(fd);
        waitpid(pid, NULL, 0);
        signal(SIGINT, SIG_IGN);
    }
}


