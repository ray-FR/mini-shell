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
void builtin_redirect(struct command* cmd, char* fileName, int typeOfRedirection);


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
    getcwd(wd, 512); // No checks due to weird behaviour found on my linux VM, where if statement gets triggered even with the command returning 0, thus exiting the program even if it was successful. 
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
    char* redirect;
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

        if (((redirect = strchr(buf, '>')) != NULL) || ((redirect = strchr(buf, '<')) != NULL)){
            int typeOfR;

            if (*(redirect) == '<'){
                firstCmd = strndup(buf, (redirect - buf));
                secondCmd = isspace(*(redirect+1)) ? strndup(redirect+2, strlen(buf)) : strndup(redirect+1, strlen(buf));
                cmd = parse_cmd(firstCmd, strlen(firstCmd));
                cmd2 = parse_cmd(secondCmd, strlen(secondCmd));
                typeOfR = 2;
 
            }

            else if (*(redirect+1) == '>'){ // If '>>' within buffer
                firstCmd = strndup(buf, (redirect - buf));
                secondCmd = isspace(*(redirect+2)) ? strndup(redirect+3, strlen(buf)) : strndup(redirect+2, strlen(buf)); 
                cmd = parse_cmd(firstCmd, strlen(firstCmd));
                cmd2 = parse_cmd(secondCmd, strlen(secondCmd));
                typeOfR = 0;

            }
            
            
            else {
                firstCmd = strndup(buf, (redirect - buf));
                secondCmd = isspace(*(redirect+1)) ? strndup(redirect+2, strlen(buf)) : strndup(redirect+1, strlen(buf));
                cmd = parse_cmd(firstCmd, strlen(firstCmd));
                cmd2 = parse_cmd(secondCmd, strlen(secondCmd));
                typeOfR = 1;
            }

            replaceEnvVar(EV, cmd);
            replaceEnvVar(EV, cmd2);
            builtin_redirect(cmd, cmd2->argv[0], typeOfR);
            free_cmd(cmd2);
        }
        

        else if ((pipe = strchr(buf, '|')) != NULL){
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
                printf("\nDo you want to search for commands within the shell? (yes/anything else for no) ");
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