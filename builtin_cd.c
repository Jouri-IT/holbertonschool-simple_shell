#include "shell.h"

/**
 * builtin_cd - changes the current directory of the process
 * @args: array of arguments (args[0] = "cd", args[1] = directory)
 * @env: pointer to the environment
 * @shell_name: name of the shell (argv[0])
 *
 * Return: 0 on success, 1 on failure
 */
int builtin_cd(char **args, char **env, char *shell_name)
{
	char *dir = NULL;
	char cwd[1024];
	char oldpwd[1024];

	(void)env;

	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
	{
		perror("getcwd");
		return (1);
	}

	if (args[1] == NULL || _strcmp(args[1], "~") == 0)
	{
		dir = _getenv("HOME");
		if (dir == NULL)
		{
			fprintf(stderr, "%s: 1: cd: HOME not set\n", shell_name);
			return (1);
		}
	}
	else if (_strcmp(args[1], "-") == 0)
	{
		dir = _getenv("OLDPWD");
		if (dir == NULL)
		{
			fprintf(stderr, "%s: 1: cd: OLDPWD not set\n",
				shell_name);
			return (1);
		}
		write(STDOUT_FILENO, dir, strlen(dir));
		write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		dir = args[1];
	}

	if (chdir(dir) == -1)
	{
		fprintf(stderr, "%s: 1: cd: can't cd to %s\n",
			shell_name, dir);
		return (1);
	}

	_setenv("OLDPWD", oldpwd);

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		_setenv("PWD", cwd);

	return (0);
}
