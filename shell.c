#include "main.h"

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
	char *token;
	(void)ac;

	while (1)
	{
		/* Display prompt only in interactive mode */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1) /* Handle End of File (Ctrl+D) */
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* Tokenize the input to remove spaces, tabs, and newlines */
		token = strtok(line, " \n\t\r");

		/* Execute only if the command is not empty */
		if (token != NULL)
		{
			if (strcmp(token, "exit") == 0)
				break;
			execute_command(token, av[0]);
		}
	}
	free(line);
	return (0);
}
