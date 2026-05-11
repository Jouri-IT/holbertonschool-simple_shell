#include "main.h"

alias_t *aliases = NULL;

/**
 * print_aliases - Prints all aliases or specific ones
 * @name: Name of specific alias (NULL for all)
 */
void print_aliases(char *name)
{
	alias_t *temp = aliases;

	while (temp)
	{
		if (name == NULL || strcmp(temp->name, name) == 0)
			printf("%s='%s'\n", temp->name, temp->value);
		temp = temp->next;
	}
}

/**
 * set_alias - Defines or updates an alias
 * @name: Alias name
 * @value: Alias value
 */
void set_alias(char *name, char *value)
{
	alias_t *temp = aliases;

	while (temp)
	{
		if (strcmp(temp->name, name) == 0)
		{
			free(temp->value);
			temp->value = strdup(value);
			return;
		}
		temp = temp->next;
	}
	temp = malloc(sizeof(alias_t));
	if (temp == NULL)
		return;
	temp->name = strdup(name);
	temp->value = strdup(value);
	temp->next = aliases;
	aliases = temp;
}

/**
 * handle_alias - Main handler for the alias builtin
 * @args: Command arguments
 */
void handle_alias(char **args)
{
	int i;
	char *equal;

	if (args[1] == NULL)
	{
		print_aliases(NULL);
		return;
	}
	for (i = 1; args[i]; i++)
	{
		equal = strchr(args[i], '=');
		if (equal)
		{
			*equal = '\0';
			set_alias(args[i], equal + 1);
		}
		else
			print_aliases(args[i]);
	}
}

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
 * run_cmd - Parses and runs a single command including builtins
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
	if (strcmp(args[0], "alias") == 0)
	{
		handle_alias(args);
		return (0);
	}
	return (execute_command(args, prog_name));
}

/**
 * main - Shell with support for ; && || and alias
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
