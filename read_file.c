#include "shell.h"

/**
 * read_from_file - Reads a line from a file descriptor
 * @fd: The file descriptor to read from
 *
 * Return: A string containing the line read, or NULL on EOF
 */
char *read_from_file(int fd)
{
	static char buf[1024];
	static int pos, bytes;
	char *line = NULL;
	int i = 0;

	line = malloc(1024);
	if (!line)
		return (NULL);

	while (1)
	{
		if (pos >= bytes)
		{
			bytes = read(fd, buf, 1024);
			pos = 0;
			if (bytes <= 0)
			{
				if (i == 0)
				{
					free(line);
					return (NULL);
				}
				break;
			}
		}
		line[i] = buf[pos++];
		if (line[i] == '\n')
		{
			line[i] = '\0';
			break;
		}
		i++;
	}
	line[i] = '\0';
	return (line);
}
