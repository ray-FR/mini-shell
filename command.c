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
        if (!isspace(buf[i]) && isspace(prev)) { 
            beg = i; 
            if (strcmp(buf[i], "\"") == 0){
                isAQuote = 1;
            }
        }
        else if (isspace(buf[i]) && !isspace(prev) && !isAQuote) {
            cmd->argv[cmd->argc++] = strndup(buf + beg, i - beg);
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