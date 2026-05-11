#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/* Access to the system environment */
extern char **environ;

/* Function prototypes */
void execute_command(char *command, char *prog_name);

#endif
