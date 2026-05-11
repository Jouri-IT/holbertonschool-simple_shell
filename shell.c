#include "main.h"

alias_t *aliases = NULL;

/**
 * handle_variables - Replaces variables like $?, $$, and $ENV
 * @args: Array of arguments
 * @last_s: Exit status of the last command
 */
void handle_variables(char **args, int last_s)
{
	int i;
	char *val;
	char pid_s[20], status_s[20];

	sprintf(pid_s, "%d", getpid());
	sprintf(status_s, "%d", last_s);

	for (i = 0; args[i]; i++)
	{
		if (args[i][0] == '$' && args[i][1] != '\0')
		{
			if (strcmp(args[i], "$?") == 0)
				args[i] = strdup(status_s);
			else if (strcmp(args[i], "$$") == 0)
				args[i] = strdup(pid_s);
			else
			{
				val = getenv(args[i] + 1);
				if (val)
					args[i] = strdup(val);
				else
					args[i] = strdup("");
			}
		}
	}
}

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
 * set_alias - Defines or updates an alias at the end of the list
 * @name: Alias name
 * @value: Alias value
 */
void set_alias(char *name, char *value)
{
	alias_t *temp = find_alias(name);
	alias_t *new_node, *last;

	if (temp)
	{
		free(temp->value);
		temp->value = strdup(value);
		return;
	}
	new_node = malloc(sizeof(alias_t));
	if (new_node == NULL)
		return;
	new_node->name = strdup(name);
	new_node->value = strdup(value);
	new_node->next = NULL;
	if (aliases == NULL)
		aliases = new_node;
	else
	{
		last = aliases;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
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
 * run_cmd - Parses and runs a single command including expansions
 * @cmd_str: Command string
 * @prog_name: Program name
 * @last_s: Reference to last exit status
 * Return: exit status
 */
int run_cmd(char *cmd_str, char *prog_name, int last_s)
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
		return (last_s);

	while (loop < 10)
	{
		temp = find_alias(args[0]);
		if (temp)
			args[0] = temp->value;
		else
			break;
		loop++;
	}

	handle_variables(args, last_s);

	if (strcmp(args[0], "exit") == 0)
	{
		free_aliases();
		exit(last_s);
	}
	if (strcmp(args[0], "alias") == 0)
	{
		handle_alias(args);
		return (0);
	}
	return (execute_command(args, prog_name));
}

/**
 * main - Shell with support for ;, &&, ||, alias and variables
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
					last_s = run_cmd(next_cmd, av[0], last_s);
					break;
				}
				*op = '\0';
				last_s = run_cmd(next_cmd, av[0], last_s);
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
