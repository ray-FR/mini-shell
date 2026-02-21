#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "command.h"

#define NB_BUILTINS 2

int builtin_exit(struct command* cmd);
int builtin_cd(struct command* cmd);
int exec_cmd(struct command* cmd);
void exec_piped_cmds(struct command* cmd1, struct command* cmd2);


struct builtin {
    char *name;
    int (*func)(struct command* cmd);
} builtins[NB_BUILTINS] = {
    { "exit", builtin_exit},
    { "cd", builtin_cd},
};


char* getFullLogin(void){
    char* name;
    char host[256];
    char cwd[256];

    if ((name = getlogin()) == NULL){
        perror("name");
        exit(1);
    }
    if (gethostname(host, 256) < 0){
        perror("gethostname");
        exit(2);
    }
    if (getcwd(cwd, 256) < 0){
        perror("getcwd");
        exit(3);
    }
    
    strcat(name, "@");
    strcat(name, host);
    strcat(name, ":");
    strcat(name, cwd);
    
    return name;
}

int find_builtin(char* command){
    for (int i = 0; i < NB_BUILTINS; i++){
        if (strcmp(command, builtins[i].name) == 0){
            return i;
        }
    }
    return -1;

}

int main(){
    char* fullLogin;
    char* pipe;
    char* firstCmd;
    char* secondCmd;


    char buf[256];
    int commandIndex;
    struct command* cmd;
    struct command* cmd2;

    fullLogin = getFullLogin();
    signal(SIGINT, SIG_IGN);

    while(1){
        
        printf("%s ", fullLogin);
        fgets(buf, 256, stdin);
        if ((pipe = strchr(buf, '|')) != NULL){
            firstCmd = strndup(buf, pipe-buf); 
            secondCmd = strndup(pipe+1, strlen(buf)); 
            cmd = parse_cmd(firstCmd, strlen(firstCmd));
            cmd2 = parse_cmd(secondCmd, strlen(secondCmd));
            exec_piped_cmds(cmd, cmd2);
            free_cmd(cmd2);
            strcpy(firstCmd, "");
            strcpy(secondCmd, "");
        }
        else {
            cmd = parse_cmd(buf, strlen(buf));
            commandIndex = find_builtin(cmd->argv[0]);
            if (commandIndex >= 0){
                builtins[commandIndex].func(cmd);
                strcpy(fullLogin, "");
                fullLogin = getFullLogin();
            }
            else{
                printf("mini-shell: built_in command not found: %s\n", buf);
                printf("Available built-in commands:\n");
                for (int i = 0; i < NB_BUILTINS; i++){
                    printf("\t%s\n", builtins[i].name);
                }
                printf("\nDo you want to search for commands within the shell? (yes/no) ");
                strcpy(buf, "");
                fgets(buf, 256, stdin);
                if (strncmp(buf, "yes", 3) == 0){
                    if (exec_cmd(cmd) != 0) printf("bash: command not found: %s\n", buf);
                }

            }}
        strcpy(buf, "");
        free_cmd(cmd);
    }




    return 0;
}