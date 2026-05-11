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
 * run_single_command - Splits a single command into args and executes it
 * @cmd_str: The command string (potentially with spaces)
 * @prog_name: Name of the shell
 */
void run_single_command(char *cmd_str, char *prog_name)
{
	char *args[1024];
	char *token;
	int i = 0;

	token = strtok(cmd_str, " \n\t\r");
	while (token != NULL)
	{
		args[i++] = token;
		token = strtok(NULL, " \n\t\r");
	}
	args[i] = NULL;

	if (args[0] != NULL)
	{
		if (strcmp(args[0], "exit") == 0)
			exit(0);
		execute_command(args, prog_name);
	}
}

/**
 * main - Simple UNIX command line interpreter with ; handling
 * @ac: Argument count
 * @av: Argument vector
 *
 * Return: Always 0
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *commands[1024];
	char *cmd_token;
	int j;
	(void)ac;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			break;
		}

		/* First split: by semicolon ; */
		j = 0;
		cmd_token = strtok(line, ";\n");
		while (cmd_token != NULL)
		{
			commands[j++] = cmd_token;
			cmd_token = strtok(NULL, ";\n");
		}
		commands[j] = NULL;

		/* Execute each command sequentially */
		for (j = 0; commands[j] != NULL; j++)
		{
			run_single_command(commands[j], av[0]);
		}
	}
	free(line);
	return (0);
}
