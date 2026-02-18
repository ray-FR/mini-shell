#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int builtin_cd(int argc, char** argv){

    if(chdir(argv[argc-1]) < 0){
        perror("chdir");
        return 1;
    }
    return 0;
}