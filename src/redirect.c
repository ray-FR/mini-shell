#include <fcntl.h>
#include "command.h"

void builtin_redirect(struct command* cmd, int typeOfRedirect){
    int fd, fd2;
    pid_t pid;
    switch (typeOfRedirect){
        case 0:
            if ((fd = open(cmd->argv[2], O_WRONLY)) < 0){
                perror('fd');
                exit(1);
            }

            
            break;
    }
}


