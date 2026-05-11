#include "main.h"

/**
 * main - Entry point for the simple shell 0.1
 * @ac: Argument count (unused)
 * @av: Argument vector (used for error messages)
 *
 * Return: Always 0
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	(void)ac;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1) /* Handle Ctrl+D */
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (strlen(line) > 0)
			execute_command(line, av[0]);
	}
	free(line);
	return (0);
}

/**
 * execute_command - Forks a process to execute a command
 * @command: The command to execute (full path)
 * @prog_name: Name of the shell program for errors
 */
void execute_command(char *command, char *prog_name)
{
	pid_t child_pid;
	int status;
	char *args[2];

	args[0] = command;
	args[1] = NULL;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		return;
	}
	if (child_pid == 0) /* Child process */
	{
		if (execve(command, args, environ) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", prog_name, command);
			exit(EXIT_FAILURE);
		}
	}
	else /* Parent process */
	{
		wait(&status);
	}
}
