#include "main.h"

/**
 * main - Entry point for the simple shell
 * @ac: argument count
 * @av: argument vector
 * Return: exit status
 */
int main(int ac, char **av)
{
	char *line = NULL, *cmd_path;
	size_t len = 0;
	ssize_t read_bytes;
	char *args[64];
	int child_status, i, exit_status = 0;
	pid_t child;

	(void)ac;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);
		read_bytes = getline(&line, &len, stdin);
		if (read_bytes == -1)
		{
			free(line);
			exit(exit_status);
		}
		for (i = 0; i < 64; i++)
			args[i] = NULL;
		args[0] = strtok(line, " \t\n");
		if (!args[0])
			continue;

		/* TASK 5: exit built-in */
		if (strcmp(args[0], "exit") == 0)
		{
			free(line);
			exit(exit_status);
		}
		/* TASK 6: env built-in */
		if (strcmp(args[0], "env") == 0)
		{
			for (i = 0; environ[i]; i++)
				printf("%s\n", environ[i]);
			continue;
		}

		for (i = 1; i < 63; i++)
		{
			args[i] = strtok(NULL, " \t\n");
			if (!args[i])
				break;
		}
		cmd_path = get_path(args[0]);
		if (!cmd_path)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", av[0], args[0]);
			exit_status = 127;
			continue;
		}
		child = fork();
		if (child == 0)
		{
			execve(cmd_path, args, environ);
			perror(av[0]);
			free(cmd_path);
			free(line);
			exit(127);
		}
		wait(&child_status);
		if (WIFEXITED(child_status))
			exit_status = WEXITSTATUS(child_status);
		free(cmd_path);
	}
	return (exit_status);
}
