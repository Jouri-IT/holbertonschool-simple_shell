#include "main.h"

/**
 * build_path - combines directory and command into a full path
 * @dir: directory path
 * @cmd: command name
 * Return: newly allocated string with full path, or NULL on failure
 */
char *build_path(char *dir, char *cmd)
{
	char *full_path;
	int dir_len = 0, cmd_len = 0;

	if (dir)
		dir_len = strlen(dir);
	if (cmd)
		cmd_len = strlen(cmd);

	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);

	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, cmd);

	return (full_path);
}

/**
 * get_path - finds the full path of a command
 * @command: command to find
 * Return: full path if found, NULL if not
 */
char *get_path(char *command)
{
	char *path, *path_copy, *token, *file_path;
	struct stat st;

	if (!command)
		return (NULL);
	/* If command contains '/', it's a direct path. Check it directly. */
	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}
	path = _getenv("PATH");
	if (!path || !*path)
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token)
	{
		file_path = build_path(token, command);
		if (stat(file_path, &st) == 0)
		{
			free(path_copy);
			return (file_path);
		}
		free(file_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
