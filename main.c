#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NB_BUILTINS 1

int builtin_exit(int, char**);


struct builtin {
    char *name;
    int (*func)(int, char **);
} builtins[NB_BUILTINS] = {
    { "exit", builtin_exit},
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

int main(int argc, char** argv){
    (void) argc;
    (void) argv;
    char userInput[256];
    char* fulllogin = getFullLogin();

    while(1){
        printf("%s ", fulllogin);
        scanf("%s", userInput);
    }




    return 0;
}