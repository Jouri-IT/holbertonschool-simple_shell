#include "main.h"

/**
 * _setenv - Sets or modifies an environment variable.
 * @name: The variable name
 * @value: The value to assign
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *name, const char *value)
{
	int i, name_len, val_len, entry_len, env_size;
	char *new_entry, *existing;

	if (!name || !*name || !value)
		return (-1);

	name_len = 0;
	while (name[name_len])
		name_len++;

	val_len = 0;
	while (value[val_len])
		val_len++;

	entry_len = name_len + val_len + 2;
	new_entry = malloc(entry_len);
	if (!new_entry)
		return (-1);

	for (i = 0; i < name_len; i++)
		new_entry[i] = name[i];
	new_entry[i++] = '=';
	for (val_len = 0; value[val_len]; val_len++)
		new_entry[i++] = value[val_len];
	new_entry[i] = '\0';

	for (i = 0; environ[i]; i++)
	{
		existing = environ[i];
		if (_strncmp(existing, name, name_len) == 0
			&& existing[name_len] == '=')
		{
			environ[i] = new_entry;
			return (0);
		}
	}

	env_size = i;
	{
		char **new_env = malloc(sizeof(char *) * (env_size + 2));
		int j;

		if (!new_env)
		{
			free(new_entry);
			return (-1);
		}
		for (j = 0; j < env_size; j++)
			new_env[j] = environ[j];
		new_env[env_size] = new_entry;
		new_env[env_size + 1] = NULL;
		environ = new_env;
	}
	return (0);
}

/**
 * _unsetenv - Removes an environment variable.
 * @name: The variable name to remove
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(const char *name)
{
	int i, name_len, found = 0;

	if (!name || !*name)
		return (-1);

	name_len = 0;
	while (name[name_len])
		name_len++;

	for (i = 0; environ[i]; i++)
	{
		if (!found && _strncmp(environ[i], name, name_len) == 0
			&& environ[i][name_len] == '=')
		{
			found = 1;
		}
		if (found)
			environ[i] = environ[i + 1];
	}

	return (found ? 0 : -1);
}
