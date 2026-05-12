/* Assuming you have parsed the user input into an array called 'args' */
char *actual_path;
pid_t child_pid;
int status;

if (args[0] == NULL)
	continue; /* Skip empty lines */

actual_path = get_path(args[0]);

if (actual_path == NULL)
{
	/* Print the exact error format sh expects */
	fprintf(stderr, "%s: 1: %s: not found\n", argv[0], args[0]);
	/* DO NOT FORK HERE */
}
else
{
	child_pid = fork();
	if (child_pid == 0)
	{
		if (execve(actual_path, args, environ) == -1)
		{
			perror(argv[0]);
			free(actual_path);
			exit(1);
		}
	}
	else
	{
		wait(&status);
		free(actual_path); /* Free the memory from get_path */
	}
}
