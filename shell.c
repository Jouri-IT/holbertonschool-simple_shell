#include "main.h"

alias_t *aliases = NULL;

/**
 * free_aliases - Frees the alias linked list
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
 * handle_alias - Handles the alias builtin command
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
				if (!aliases)
					aliases = new_n;
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
 * _getenv - Custom getenv (forbidden function replacement)
 */
char *_getenv(char *name)
{
	int i = 0, len = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * find_path - Finds command in PATH
 */
char *find_path(char *cmd)
{
	char *path = _getenv("PATH"), *path_cp, *tok, *fp;
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
 * handle_variables - Handles $?, $$ and environment variables
 */
void handle_variables(char **args, int last_s)
{
	int i;
	char *val, p[20], s[20], *new_v;

	sprintf(p, "%d", getpid());
	sprintf(s, "%d", last_s);
	for (i = 0; args[i]; i++)
	{
		if (args[i][0] == '$' && args[i][1])
		{
			if (strcmp(args[i], "$?") == 0)
				new_v = strdup(s);
			else if (strcmp(args[i], "$$") == 0)
				new_v = strdup(p);
			else
			{
				val = _getenv(args[i] + 1);
				new_v = strdup(val ? val : "");
			}
			args[i] = new_v;
		}
	}
}

/**
 * run_cmd - Executes a single command
 */
int run_cmd(char *cs, char *pn, int ls)
{
	char *args[1024], *tk, *cp;
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
	handle_variables(args, ls);
	if (!strcmp(args[0], "exit")) { free_aliases(); exit(ls); }
	if (!strcmp(args[0], "alias")) { handle_alias(args); return (0); }
	cp = find_path(args[0]);
	pid = fork();
	if (pid == 0)
	{
		if (execve(cp, args, environ) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n", pn, args[0]);
			free(cp); exit(127);
		}
	} else { waitpid(pid, &st_val, 0); free(cp); }
	return (WIFEXITED(st_val) ? WEXITSTATUS(st_val) : ls);
}

/**
 * main - Shell main loop
 */
int main(int ac, char **av)
{
	char *line = NULL, *cmds[1024], *tk;
	size_t len = 0;
	int i, j, last_s = 0;
	(void)ac;

	while (getline(&line, &len, stdin) != -1)
	{
		i = 0;
		tk = strtok(line, ";\n");
		while (tk) { cmds[i++] = tk; tk = strtok(NULL, ";\n"); }
		cmds[i] = NULL;
		for (j = 0; cmds[j]; j++)
		{
			char *next = cmds[j];
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
		}
	}
	free_aliases(); free(line);
	return (last_s);
}
