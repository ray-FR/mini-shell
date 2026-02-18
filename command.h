#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct command {
    int argc;
    char **argv;
};

int count_args(const char* s);
struct command* parse_cmd(const char* buf, int len);
void free_cmd(struct command* cmd);



#endif