#include <fcntl.h>
#include "command.h"



void builtin_redirect(struct command* cmd, char* fileName, int typeOfRedirection){
    int fd;
    pid_t pid;
    
    
    if ((fd = open(fileName, O_RDWR, S_IRUSR | S_IWUSR)) < 0){
        perror("fd");
        exit(1);
    }
    
    if ((pid = fork()) < 0){
        perror("fork");
        exit(1);
    }
    if (pid == 0){
        
        if (typeOfRedirection == 2){
            char buf[1024];
            /* 
            https://stackoverflow.com/a/68156485
            Copied, but modified
            Mon code ne marchait pas jusqu'à que j'implémente ce code trouvé dans cette réponse stackoverflow. Pour être honnête je ne suis pas trop habitué à l'utilisation des fonctions de type read().
            */
            ssize_t n = 0;
            buf[1023] = '\0';
            while ((n = read(fd, buf, 1024)) > 0){
                if (n == -1){
                    perror("read");
                    exit(1);
                }
            }
            buf[strlen(buf)] = '\n';
            struct command* a = parse_cmd(buf, strlen(buf));
        }
        
        
        if (typeOfRedirection == 0){
            if (lseek(fd, 0, SEEK_END) < 0){
                perror("lseek");
                exit(1);
            }
        }
        
        else if (typeOfRedirection == 1){    
            if (ftruncate(fd, 0) == 0) 
            lseek(fd, 0, SEEK_SET); 
            else {
                perror("ftruncate");
                exit(1);
            }
        }
        
        
        
        
        if (dup2(fd, 1) < 0) {
            perror("dup2");
            exit(1);
        }
        if (close(fd) < 0){
            perror("close");
            exit(1);
        }
        
        signal(SIGINT, SIG_DFL);
        execvp(cmd->argv[0], &cmd->argv[0]);
        perror("execvp");
        exit(1);
    }
    else{
        close(fd);
        waitpid(pid, NULL, 0);
        signal(SIGINT, SIG_IGN);
    }
}


