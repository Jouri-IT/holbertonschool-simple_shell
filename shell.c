#include "main.h"

/**
 * main - entry point for the shell
 * @argc: arg count
 * @argv: arg vector
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
    char *line = NULL;
    size_t len = 0;
    char *args[1024]; /* Or however you are storing your tokens */
    char *actual_path;
    int status;
    pid_t child_pid;

    (void)argc;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            printf("($) ");

        if (getline(&line, &len, stdin) == -1)
            break;

        /* --- 1. YOUR TOKENIZATION LOGIC HERE --- */
        /* (The part where you use strtok to fill the args array) */
        /* Example: args[0] = strtok(line, " \n"); */

        if (args[0] == NULL)
            continue;

        /* --- 2. THE PATH CHECK (Fixing your error) --- */
        actual_path = get_path(args[0]);

        if (actual_path == NULL)
        {
            /* This matches the exact error output required */
            fprintf(stderr, "%s: 1: %s: not found\n", argv[0], args[0]);
            continue; /* Skip to next prompt, do NOT fork */
        }

        /* --- 3. THE FORK --- */
        child_pid = fork();
        if (child_pid == 0)
        {
            if (execve(actual_path, args, environ) == -1)
            {
                perror("Error");
                exit(1);
            }
        }
        else
        {
            wait(&status);
            free(actual_path); /* Avoid the memory leak! */
        }
    }
    free(line);
    return (0);
}
