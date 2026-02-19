#include <unistd.h>
#include <sys/wait.h>
#include "command.h"

int exec_cmd(struct command* cmd){
    pid_t pid;

    pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        execvp(cmd->argv[0], &(cmd->argv[0]));
        perror("execvp");
        
    } else waitpid(pid, NULL, 0);
    return 0;

    
    
}