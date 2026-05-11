#include "main.h"

/**
 * execute_command - Executes a command and returns exit status
 * @args: Arguments array
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
 * run_cmd - Parses and runs a single command
 * @cmd_str: Command string
 * @prog_name: Program name
 * Return: exit status
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
 * main - Simple shell with full logic support (&&, ||, ;)
 * @ac: count, @av: vector
 * Return: last status
 */
int main(int ac, char **av)
{
	char *line = NULL, *cmd, *next_cmd;
	size_t len = 0;
	int last_s = 0;
	(void)ac;

	while (getline(&line, &len, stdin) != -1)
	{
		cmd = strtok(line, ";\n");
		while (cmd)
		{
			next_cmd = cmd;
			while (*next_cmd)
			{
				char *and = strstr(next_cmd, "&&");
				char *or = strstr(next_cmd, "||");
				char *op = (and && (!or || and < or)) ? and : or;

				if (!op)
				{
					last_s = run_cmd(next_cmd, av[0]);
					break;
				}
				*op = '\0';
				last_s = run_cmd(next_cmd, av[0]);
				if ((op == and && last_s != 0) || (op == or && last_s == 0))
					break;
				next_cmd = op + 2;
			}
			cmd = strtok(NULL, ";\n");
		}
	}
	free(line);
	return (last_s);
}
