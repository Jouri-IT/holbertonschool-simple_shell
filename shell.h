#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

extern char **environ;

int shell_loop(int fd, char **av);
char *build_path(char *dir, char *cmd);
char *get_path(char *command);
char *_getenv(const char *name);
int _setenv(const char *name, const char *value);
int _unsetenv(const char *name);
int builtin_cd(char **args, char **env);
int _strlen(const char *s);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);
char *_strchr(char *s, char c);
char *_strdup(char *str);
char *_memcpy(char *dest, char *src, size_t n);
char *_strtok(char *str, const char *delim);
char *_strcpy(char *dest, const char *src);
int _atoi(const char *s);
int _is_valid_exit(const char *s);
ssize_t _getline(char **lineptr, size_t *n);

#endif /* SHELL_H */
