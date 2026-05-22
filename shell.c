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

		/* Use our custom _getline instead of standard getline */
		read_bytes = _getline(&line, &len);
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

		/* TASK 5: exit built-in (using custom _strcmp) */
		if (_strcmp(args[0], "exit") == 0)
		{
			free(line);
			exit(exit_status);
		}

		/* TASK 6: env built-in (using custom _strcmp) */
		if (_strcmp(args[0], "env") == 0)
		{
			/* Assuming your print_env helper logic is handled here or called below */
			continue;
		}
	}
	return (exit_status);
}
