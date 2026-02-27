#include <fcntl.h>
#include "command.h"

void builtin_redirect(struct command* cmd, int typeOfRedirect){
    int fd, fd2;
    switch (typeOfRedirect){
        case 0:
            if ((fd = open(cmd->argv[0], O_RDONLY)) < 0){
                
            }
            break;
    }
}