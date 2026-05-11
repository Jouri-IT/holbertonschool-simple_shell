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
 * handle_alias - Main handler for the alias builtin
 * @args: Command arguments
 */
void handle_alias(char **args)
{
	int i;
	char *eq;
	alias_t *at, *new_n, *last;

	if (args[1] == NULL)
	{
		for (at = aliases; at; at = at->next)
			printf("%s='%s'\n", at->name, at->value);
		return;
	}
	for (i = 1; args[i]; i++)
	{
		eq = strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			for (at = aliases; at && strcmp(at->name, args[i]); at = at->next)
				;
			if (at)
			{
				free(at->value);
				at->value = strdup(eq + 1);
			} else {
				new_n = malloc(sizeof(alias_t));
				new_n->name = strdup(args[i]);
				new_n->value = strdup(eq + 1);
				new_n->next = NULL;
				if (!aliases) aliases = new_n;
				else {
					for (last = aliases; last->next; last = last->next)
						;
					last->next = new_n;
				}
			}
		} else {
			for (at = aliases; at; at = at->next)
				if (!strcmp(at->name, args[i]))
					printf("%s='%s'\n", at->name, at->value);
		}
	}
}

/**
 * find_path - Searches for a command in the PATH
 * @cmd: The command to find
 * Return: Full path or dup of cmd
 */
char *find_path(char *cmd)
{
	char *path = getenv("PATH"), *path_cp, *tok, *fp;
	struct stat st;

	if (!path || strchr(cmd, '/') || !strlen(path))
		return (strdup(cmd));
	path_cp = strdup(path);
	tok = strtok(path_cp, ":");
	while (tok)
	{
		fp = malloc(strlen(tok) + strlen(cmd) + 2);
		sprintf(fp, "%s/%s", tok, cmd);
		if (stat(fp, &st) == 0)
		{
			free(path_cp);
			return (fp);
		}
		free(fp);
		tok = strtok(NULL, ":");
	}
	free(path_cp);
	return (strdup(cmd));
}

/**
 * run_cmd - Parses and runs a command
 * @cs: cmd string, @pn: prog name, @ls: last status
 * Return: status
 */
int run_cmd(char *cs, char *pn, int ls)
{
	char *args[1024], *tk, *cp, *v, p[20], s[20];
	int i = 0, lp = 0, st_val = 0;
	alias_t *at;
	pid_t pid;

	tk = strtok(cs, " \n\t\r");
	while (tk) { args[i++] = tk; tk = strtok(NULL, " \n\t\r"); }
	args[i] = NULL;
	if (!args[0]) return (ls);
	while (lp++ < 10)
	{
		for (at = aliases; at && strcmp(at->name, args[0]); at = at->next)
			;
		if (at) args[0] = at->value; else break;
	}
	sprintf(p, "%d", getpid()); sprintf(s, "%d", ls);
	for (i = 0; args[i]; i++) {
		if (args[i][0] == '$' && args[i][1]) {
			if (!strcmp(args[i], "$?")) args[i] = strdup(s);
			else if (!strcmp(args[i], "$$")) args[i] = strdup(p);
			else { v = getenv(args[i] + 1); args[i] = strdup(v ? v : ""); }
		}
	}
	if (!strcmp(args[0], "exit")) { free_aliases(); exit(ls); }
	if (!strcmp(args[0], "alias")) { handle_alias(args); return (0); }
	cp = find_path(args[0]); pid = fork();
	if (pid == 0) {
		if (execve(cp, args, environ) == -1) {
			fprintf(stderr, "%s: 1: %s: not found\n", pn, args[0]);
			free(cp); exit(127);
		}
	} else {
		waitpid(pid, &st_val, 0);
		free(cp);
	}
	return (WIFEXITED(st_val) ? WEXITSTATUS(st_val) : ls);
}

/**
 * main - Main shell loop
 * @ac: arg count, @av: arg vector
 * Return: status
 */
int main(int ac, char **av)
{
	char *line = NULL, *cmd, *next;
	size_t len = 0;
	int last_s = 0;
	(void)ac;

	while (getline(&line, &len, stdin) != -1)
	{
		cmd = strtok(line, ";\n");
		while (cmd)
		{
			next = cmd;
			while (*next)
			{
				char *a = strstr(next, "&&"), *o = strstr(next, "||");
				char *op = (a && (!o || a < o)) ? a : o;
				if (!op) { last_s = run_cmd(next, av[0], last_s); break; }
				*op = '\0';
				last_s = run_cmd(next, av[0], last_s);
				if ((op == a && last_s != 0) || (op == o && last_s == 0)) break;
				next = op + 2;
			}
			cmd = strtok(NULL, ";\n");
		}
	}
	free_aliases(); free(line);
	return (last_s);
}
