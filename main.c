#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* getfulllogin(void){
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
    fprintf(stderr, "%s\n", name);
    
    return name;
}

int main(int argc, char** argv){

    return 0;
}