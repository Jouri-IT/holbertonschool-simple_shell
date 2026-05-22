#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern char **environ;

char *build_path(char *dir, char *cmd);
char *get_path(char *command);
char *_getenv(const char *name);
ssize_t _getline(char **lineptr, size_t *n);
int _strcmp(char *s1, char *s2);
char *_strchr(char *s, char c);
char *_strdup(char *str);
char *_memcpy(char *dest, char *src, size_t n);
char *_strtok(char *str, const char *delim);
int _atoi(const char *s);
int _is_valid_exit(const char *s);

#endif
