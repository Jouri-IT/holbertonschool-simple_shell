#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

typedef struct alias_s {
    char *name;
    char *value;
    struct alias_s *next;
} alias_t;

#endif
