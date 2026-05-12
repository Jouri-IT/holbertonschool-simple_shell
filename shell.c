/* ... inside your loop where you handle commands ... */

char *cmd = args[0];
char *actual_path = get_path(cmd);

if (actual_path == NULL)
{
    /* Print error exactly as sh does: program_name: 1: cmd: not found */
    fprintf(stderr, "%s: 1: %s: not found\n", argv[0], cmd);
    /* DO NOT FORK HERE */
}
else
{
    pid_t child_pid = fork();
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
        free(actual_path); /* Free the string allocated by get_path */
    }
}
