# mini-shell
---
A university project to create a mini-shell using low-level system functions

## Features
- Single and double quote support
- Export Environment Variable support (+ ability to create them)
- Redirect operators (\>, \>\>) support
- Basic pipe support (2 args)

## Overview of the repository
```
src/
    main.c
    cd.c 
    command.c 
    command.h
    envvar.c
    envvar.h
    exec.c
    exec_piped_cmds.c
    exit.c
    export.c
    redirect.c
obj/
    ./src/*.c -> ./obj/*.o

.gitignore
Makefile
README.md
./mini-shell
```

## Quickstart
`make && ./mini-shell`

## Built-in structs used
```c
//main.c
#define NB_BUILTINS 3

struct builtin {
    char *name;
    int (*func)(struct command* cmd);
} builtins[NB_BUILTINS] = {
    { "exit", builtin_exit},
    { "cd", builtin_cd},
    { "export", NULL},
};

//command.h
struct command {
    int argc;
    char **argv;
};

//envvar.h
struct EVAR {
    char ENVVAR[80];
    char value[512];
};
struct envVar_t {
    int nbVar;
    int maxNumber;
    struct EVAR** vars;
};


```

## Functions
```
Structs----
    command.c (command.h)
        -> int count_args(const char* s);
            - Return number of args separated by a space.

        -> struct command* parse_cmd(const char* buf, int len);
            - Parse commands within the const char* buf by examining every char in it. Returns a pointer to a struct command. If it fails, it exit with error code 1 + errno value returned by one of the functions used within the function.

        -> void free_cmd(struct command* cmd)
            - Free the struct command pointed at.

    envvar.c (envvar.h)
        -> struct envVar_t * initEnvVar();
            - Returns pointer to a struct envVar_t, which also contains basic environment variables. If it fails, exit with error code 1 + errno value returned by one of the functions used within the function.
        -> void replaceEnvVar(struct envVar_t* EV, struct command* cmd);
            - Replaces char* within cmd with their environment variables values if found.

        -> void updateEnvVar(struct envVar_t* EV);
            - Updates basic environment variables if, for example cwd is changed.

        -> void freeEnvVar(struct envVar_t* EV);
            - Free the used environment variable struct.
----




    main.c 
        -> void getFullLogin(void);
            - Outputs to stderr a "terminal-ish" login info. Called at every REPL loop. If one of the "get functions" fails, program exits with the errno value returned by one of the functions used within the function.

        -> int find_builtin(char* command);
            - "getter" function to get the index of one of the builtin commands present within the builtins struct. If command->argv[0] matches one of the built-in commands, the function returns it's corresponding index. If no matches are found, returns -1.


        -> int main();
            (NOTE: just a very brief overview of every steps within the program)
            -> Get user input with fgets, gets stored within buf
                -> if '>' is found within buf:
                    -> if first '>' is followed by another '>':
                        -runs builtin_redirect with typeOfRedirection = 0.
                        -free_cmd for cmd2

                    -> else (in the scenario of a '>' redirection):
                        -runs builtin_redirect with typeOfRedirection = 1.
                        -free_cmd for cmd2


                -> else if '|' is found within buf:
                        -runs exec_piped_cmds


                -> else
                    -> if command found within builtins (with find_builtin)
                        - run builtin command

                    -> else
                        -> Asks user if he wishes to run command within the PATH.
                            -> If yes, runs exec_cmd
                            -> Anything else, finishes loop
            -> clears buffer
            -> free_cmd with cmd
            -> update environment variables

        ->freeEnvVar with EV;


    exec.c  
        -> int exec_cmd(struct command* cmd);
            - Execute commands in the PATH. Returns 0 if successful, or 1 if there is an error alongside the errno value returned by one of the functions used within the function.


    exec_piped_cmds.c
        -> void exec_piped_cmds(struct command* cmd1, struct command* cmd2);
            - Execute one command, which then gets piped to the input of the second command. Returns 0 if successful, or 1 if there is an error alongside the errno value returned by one of the functions used within the function.
    

    export.c
        -> int builtin_export(struct command* cmd, struct envVar_t* EV);
            - initialise a "Environment Variable" of the mini-shell. Environment variable and it's value is taken from cmd and is put in EV. Environment variable be called anything as long as it is under 80 bytes, and value can be any char* the user wants. Returns 0 if successful, or 1 if there is an error alongside the errno value returned by one of the functions used within the function.


    redirect.c
        -> void builtin_redirect(struct command* cmd, char* fileName, int typeOfRedirection);
            - Handles redirection within the mini-shell. Currently only support the '>' and '>>' redirect operators, which is handled by a unique positive integer given by typeOfRedirection (currently 0 and 1) and the file to be modified by fileName. Content redirected to fileName is given by the (valid) command contained in cmd. Exits alongside errno value of one of the functions used within the function.


    cd.c, exit.c
        -> int builtin_cd(struct command* cmd);
            - Returns 0 if successful, or 1 if there is an error alongside the errno value returned by chdir.

        -> int builtin_exit(struct command* cmd);
            - Returns 0.

```

## Limitations 
### Pipe
    - Although builtin pipe support command to be piped in right next to \'|\' (i.e `ls -l |grep ".c"`), it does not support if one of the argument to be piped to the second part of the command is right next to \'|\' (i.e `ls -l| grep ".c"`)

### Quote
    - Can only support one quoted text.
    - Does not support quoted text within double quoted text (i.e "hi 'hello' individual").

### Redirect
    - No support for \'\<\' redirect operator.