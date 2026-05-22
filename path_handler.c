#include "main.h"

/**
 * build_path - Builds a full path string from a directory and command name.
 * @dir: The directory component
 * @cmd: The command name
 * Return: Newly allocated full path string, or NULL on failure
 */
char *build_path(char *dir, char *cmd)
{
	char *full_path;
	int dir_len, cmd_len, i, j;

	if (!dir || !cmd)
		return (NULL);

	dir_len = 0;
	while (dir[dir_len])
		dir_len++;

	cmd_len = 0;
	while (cmd[cmd_len])
		cmd_len++;

	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);

	for (i = 0; i < dir_len; i++)
		full_path[i] = dir[i];

	full_path[i++] = '/';

	for (j = 0; j < cmd_len; j++)
		full_path[i + j] = cmd[j];

	full_path[i + j] = '\0';
	return (full_path);
}

/**
 * get_path - Searches PATH for a command and returns its full path.
 * @command: The command to look up
 * Return: full path if found, NULL if not
 */
char *get_path(char *command)
{
	char *path, *path_copy, *token, *file_path;
	struct stat st;

	if (!command)
		return (NULL);

	if (_strchr(command, '/'))
	{
		if (stat(command, &st) == 0)
			return (_strdup(command));
		return (NULL);
	}

	path = _getenv("PATH");
	if (!path || !*path)
		return (NULL);

	path_copy = _strdup(path);
	if (!path_copy)
		return (NULL);

	token = _strtok(path_copy, ":");
	while (token)
	{
		file_path = build_path(token, command);
		if (file_path && stat(file_path, &st) == 0)
		{
			free(path_copy);
			return (file_path);
		}
		free(file_path);
		token = _strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
