#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "command.h"
#include "envvar.h"

#define NB_BUILTINS 3

int builtin_exit(struct command* cmd);
int builtin_cd(struct command* cmd);
int exec_cmd(struct command* cmd);
int builtin_export(struct command* cmd, struct envVar_t* EV);
void exec_piped_cmds(struct command* cmd1, struct command* cmd2);


struct builtin {
    char *name;
    int (*func)(struct command* cmd);
} builtins[NB_BUILTINS] = {
    { "exit", builtin_exit},
    { "cd", builtin_cd},
    { "export", NULL},
};


void getFullLogin(void){
    char *user, host[64],wd[512];

    if ((user = getlogin()) == NULL){
        perror("getlogin");
        exit(1);
    }
    if (gethostname(host,64) < 0){
        perror("gethostname");
        exit(1);
    }
    if (getcwd(wd,512) < 0){
        perror("getcwd");
        exit(1);
    }
    fprintf(stderr,"%s@%s:%s$ ", user,host,wd);
    
    
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
    char* pipe;
    char* firstCmd;
    char* secondCmd;


    char buf[256];
    int commandIndex;
    struct command* cmd;
    struct command* cmd2;
    struct envVar_t* EV;

    signal(SIGINT, SIG_IGN);
    EV = initEnvVar();

    while(1){
        

        getFullLogin();

        fgets(buf, 256, stdin);
        if ((pipe = strchr(buf, '|')) != NULL){
            firstCmd = strndup(buf, pipe-buf); 
            secondCmd = strndup(pipe+1, strlen(buf)); 
            cmd = parse_cmd(firstCmd, strlen(firstCmd));
            replaceEnvVar(EV, cmd);
            cmd2 = parse_cmd(secondCmd, strlen(secondCmd));
            replaceEnvVar(EV, cmd2);
            exec_piped_cmds(cmd, cmd2);
            free_cmd(cmd2);
            strcpy(firstCmd, "");
            strcpy(secondCmd, "");
        }
        else {
            cmd = parse_cmd(buf, strlen(buf));
            replaceEnvVar(EV, cmd);
            commandIndex = find_builtin(cmd->argv[0]);
            if (commandIndex >= 0){
                if (commandIndex == 2) builtin_export(cmd, EV);
                else builtins[commandIndex].func(cmd);
                
                
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
        updateEnvVar(EV);
        
    }
    freeEnvVar(EV);




    return 0;
}