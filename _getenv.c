#include "shell.h"

/**
 * _getenv - gets the value of an environment variable
 * @name: name of the variable to find (e.g., "PATH")
 * Return: pointer to the value string, or NULL if not found
 */
char *_getenv(const char *name)
{
	int i, j;
	int match;

	if (!name || !environ)
		return (NULL);

	for (i = 0; environ[i]; i++)
	{
		match = 1;
		for (j = 0; name[j]; j++)
		{
			if (name[j] != environ[i][j])
			{
				match = 0;
				break;
			}
		}
		if (match && environ[i][j] == '=')
			return (&environ[i][j + 1]);
	}
	return (NULL);
}
