#include "main.h"

/**
 * main - Entry point for the simple shell
 * @ac: argument count
 * @av: argument vector
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	char *line = NULL, *cmd_path;
	size_t len = 0;
	ssize_t read_bytes;
	char *args[64];
	int status, i;
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
			exit(0);
		}
		for (i = 0; i < 64; i++)
			args[i] = NULL;
		args[0] = strtok(line, " \t\n");
		if (!args[0])
			continue;
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
			continue;
		}
		child = fork();
		if (child == 0)
		{
			execve(cmd_path, args, environ);
			perror(av[0]);
			free(cmd_path);
			free(line);
			exit(1);
		}
		wait(&status);
		free(cmd_path);
	}
	return (0);
}
