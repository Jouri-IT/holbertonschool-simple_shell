#include "main.h"

/**
 * execute_command - Forks a process to execute a command with arguments
 * @args: Array of strings containing the command and its arguments
 * @prog_name: Name of the shell program for error messages
 */
void execute_command(char **args, char *prog_name)
{
	pid_t child_pid;
	int status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		return;
	}

	if (child_pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		wait(&status);
	}
}

/**
 * main - Simple UNIX command line interpreter with arguments support
 * @ac: Argument count (unused)
 * @av: Argument vector (used for program name)
 *
 * Return: Always 0 (Success)
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *args[1024];
	char *token;
	int i;
	(void)ac;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		i = 0;
		token = strtok(line, " \n\t\r");
		while (token != NULL)
		{
			args[i++] = token;
			token = strtok(NULL, " \n\t\r");
		}
		args[i] = NULL;

		if (args[0] != NULL)
		{
			if (strcmp(args[0], "exit") == 0)
				break;
			execute_command(args, av[0]);
		}
	}
	free(line);
	return (0);
}
