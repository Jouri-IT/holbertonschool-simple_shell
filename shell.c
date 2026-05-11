#include "main.h"

/**
 * execute_command - Executes a command and returns its exit status
 * @args: Array of command and arguments
 * @prog_name: Name of the shell
 * Return: Exit status of the command
 */
int execute_command(char **args, char *prog_name)
{
	pid_t child_pid;
	int status;
	int exit_s = 0;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("Error");
		return (1);
	}
	if (child_pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", prog_name, args[0]);
			exit(127);
		}
	}
	else
	{
		waitpid(child_pid, &status, 0);
		if (WIFEXITED(status))
			exit_s = WEXITSTATUS(status);
	}
	return (exit_s);
}

/**
 * run_single_command - Splits and executes one command
 * @cmd_str: The command string
 * @prog_name: Name of the shell
 * Return: Exit status of the command
 */
int run_single_command(char *cmd_str, char *prog_name)
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
		{
			/* You might need to handle exit with arguments here later */
			exit(0);
		}
		return (execute_command(args, prog_name));
	}
	return (0);
}

/**
 * main - Simple UNIX command line interpreter with exit status support
 * @ac: Argument count
 * @av: Argument vector
 * Return: Last command exit status
 */
int main(int ac, char **av)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *commands[1024];
	char *cmd_token;
	int j, last_status = 0;
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

		j = 0;
		cmd_token = strtok(line, ";\n");
		while (cmd_token != NULL)
		{
			commands[j++] = cmd_token;
			cmd_token = strtok(NULL, ";\n");
		}
		commands[j] = NULL;

		for (j = 0; commands[j] != NULL; j++)
		{
			last_status = run_single_command(commands[j], av[0]);
		}
	}
	free(line);
	return (last_status);
}
