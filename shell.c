#include "main.h"

alias_t *aliases = NULL;

/**
 * free_aliases - Frees the memory used by the alias linked list
 */
void free_aliases(void)
{
	alias_t *temp;

	while (aliases)
	{
		temp = aliases;
		aliases = aliases->next;
		free(temp->name);
		free(temp->value);
		free(temp);
	}
}

/**
 * find_alias - Finds an alias in the list
 * @name: Name of the alias
 * Return: Pointer to the alias node or NULL
 */
alias_t *find_alias(char *name)
{
	alias_t *temp = aliases;

	while (temp)
	{
		if (strcmp(temp->name, name) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

/**
 * set_alias - Defines or updates an alias
 * @name: Alias name
 * @value: Alias value
 */
void set_alias(char *name, char *value)
{
	alias_t *temp = find_alias(name);

	if (temp)
	{
		free(temp->value);
		temp->value = strdup(value);
		return;
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
	alias_t *temp;

	if (args[1] == NULL)
	{
		temp = aliases;
		while (temp)
		{
			printf("%s='%s'\n", temp->name, temp->value);
			temp = temp->next;
		}
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
		{
			temp = find_alias(args[i]);
			if (temp)
				printf("%s='%s'\n", temp->name, temp->value);
		}
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
 * run_cmd - Parses and runs a single command including alias expansion
 * @cmd_str: Command string
 * @prog_name: Program name
 * Return: exit status
 */
int run_cmd(char *cmd_str, char *prog_name)
{
	char *args[1024], *token;
	int i = 0, loop = 0;
	alias_t *temp;

	token = strtok(cmd_str, " \n\t\r");
	while (token)
	{
		args[i++] = token;
		token = strtok(NULL, " \n\t\r");
	}
	args[i] = NULL;
	if (!args[0])
		return (0);

	/* Expansion: Replace alias with value, handle nested aliases */
	while (loop < 10) /* Prevent infinite loops */
	{
		temp = find_alias(args[0]);
		if (temp)
			args[0] = temp->value;
		else
			break;
		loop++;
	}

	if (strcmp(args[0], "exit") == 0)
	{
		free_aliases();
		exit(0);
	}
	if (strcmp(args[0], "alias") == 0)
	{
		handle_alias(args);
		return (0);
	}
	return (execute_command(args, prog_name));
}

/**
 * main - Shell with support for ; && || and alias expansion
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
	free_aliases();
	free(line);
	return (last_s);
}
