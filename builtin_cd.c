#include "shell.h"

/**
 * builtin_cd - changes the current directory of the process
 * @args: array of arguments (args[0] = "cd", args[1] = directory)
 * @env: pointer to the environment
 *
 * Return: 0 on success, 1 on failure
 */
int builtin_cd(char **args, char **env)
{
	char *dir = NULL;
	char cwd[1024];
	char *oldpwd = NULL;
	int i;

	(void)env;

	oldpwd = getcwd(cwd, sizeof(cwd));
	if (oldpwd == NULL)
	{
		perror("getcwd");
		return (1);
	}

	if (args[1] == NULL || _strcmp(args[1], "~") == 0)
	{
		dir = _getenv("HOME");
		if (dir == NULL)
		{
			write(STDERR_FILENO, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else if (_strcmp(args[1], "-") == 0)
	{
		dir = _getenv("OLDPWD");
		if (dir == NULL)
		{
			write(STDERR_FILENO, "cd: OLDPWD not set\n", 19);
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
		perror("cd");
		return (1);
	}

	_setenv("OLDPWD", oldpwd);

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		_setenv("PWD", cwd);

	(void)i;
	return (0);
}
