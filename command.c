#include "command.h"

int count_args(const char *s)
{
    int n = 0;
    char p = ' ';
    while (*s) {
        if (isspace(p) && !isspace(*s)) n++;
        p = *s++;
    }
    return n;
}

struct command * parse_cmd(const char *buf, int len)
{
    struct command *cmd = NULL;
    char prev = ' ';
    int n, beg = 0;
    int isAQuote = 0;


    if ((n = count_args(buf)) == 0) return NULL;

    cmd = malloc(sizeof(*cmd));
    cmd->argc = 0;
    cmd->argv = malloc((n + 1) * sizeof(*cmd->argv));

    for (int i = 0; i < len; i++) {
        if (!isspace(buf[i]) && isspace(prev) && (isAQuote == 0)) { 
            beg = i; 
            if (buf[i] == '\"' || buf[i] == '\''){
                isAQuote = 1;
                beg++;
                printf("ye1\n");
                
            }
        }
        else if (isspace(buf[i]) && !isspace(prev) && (isAQuote == 0)) {
            cmd->argv[cmd->argc++] = strndup(buf + beg, i - beg);
        }
        else if ((buf[i] == '\"' || buf[i] == '\'') && (isAQuote == 1)){
            cmd->argv[cmd->argc++] = strndup(buf + beg-1, (i - beg)+2);

            isAQuote = 0;
            printf("ye2\n");
        }
        prev = buf[i];
    }

    cmd->argv[cmd->argc] = NULL;
    return cmd;
}

void free_cmd(struct command* cmd){
    for(int i = 0; i<cmd->argc;i++){
        free(cmd->argv[i]);
        cmd->argv[i] = NULL;
    }
    free(cmd->argv);
    free(cmd);
}