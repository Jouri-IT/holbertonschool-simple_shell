#include "main.h"

/**
 * execute_command - Executes a command and returns status
 * @args: Command arguments
 * @prog_name: Program name
 * Return: Exit status
 */
int execute_command(char **args, char *prog_name)
{
	pid_t child_pid;
	int status, exit_s = 0;

	child_pid = fork();
	if (child_pid == -1)
		return (1);
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
 * run_cmd - Parses and runs a single command string
 * @cmd_str: The string to parse
 * @prog_name: Program name
 * Return: status
 */
int run_cmd(char *cmd_str, char *prog_name)
{
	char *args[1024], *token;
	int i = 0;

	token = strtok(cmd_str, " \n\t\r");
	while (token)
	{
		args[i++] = token;
		token = strtok(NULL, " \n\t\r");
	}
	args[i] = NULL;
	if (!args[0])
		return (0);
	if (strcmp(args[0], "exit") == 0)
		exit(0);
	return (execute_command(args, prog_name));
}

/**
 * main - Shell with ; support
 * @ac: count, @av: vectors
 * Return: Last status
 */
int main(int ac, char **av)
{
	char *line = NULL, *cmd;
	size_t len = 0;
	int last_s = 0;
	(void)ac;

	while (getline(&line, &len, stdin) != -1)
	{
		cmd = strtok(line, ";\n");
		while (cmd)
		{
			last_s = run_cmd(cmd, av[0]);
			cmd = strtok(NULL, ";\n");
		}
	}
	free(line);
	return (last_s);
}
