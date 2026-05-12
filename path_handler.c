#include "main.h"

/**
 * get_path - finds the full path of a command
 * @command: command to find (e.g. "ls")
 * Return: full path if found, NULL if not
 */
char *get_path(char *command)
{
	char *path, *path_copy, *token, *file_path;
	struct stat st;
	int cmd_len, dir_len;

	if (!command)
		return (NULL);

	/* If command is a full path (starts with / or .), check it directly */
	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0)
			return (strdup(command));
		return (NULL);
	}

	path = getenv("PATH");
	if (!path || strlen(path) == 0)
		return (NULL);

	path_copy = strdup(path);
	token = strtok(path_copy, ":");
	cmd_len = strlen(command);

	while (token != NULL)
	{
		dir_len = strlen(token);
		file_path = malloc(dir_len + cmd_len + 2);
		if (!file_path)
		{
			free(path_copy);
			return (NULL);
		}
		strcpy(file_path, token);
		strcat(file_path, "/");
		strcat(file_path, command);

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
