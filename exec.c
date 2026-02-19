#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "command.h"

int exec_cmd(struct command* cmd){
    pid_t pid;

    pid = fork();
    if (pid < 0){
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        signal(SIGINT, SIG_DFL);
        execvp(cmd->argv[0], &(cmd->argv[0]));
        perror("execvp");
        
    } else waitpid(pid, NULL, 0);
    signal(SIGINT, SIG_IGN);
    return 0;


    
}