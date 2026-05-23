#include "shell.h"

#define BUFFER_SIZE 1024

/**
 * _getline - Reads a line from a file descriptor using a static buffer.
 * @lineptr: Address of pointer to output buffer.
 * @n: Address of allocated size of *lineptr.
 * @fd: File descriptor to read from.
 * Return: Number of characters read, or -1 on EOF/error.
 */
ssize_t _getline(char **lineptr, size_t *n, int fd)
{
	static char buffer[BUFFER_SIZE];
	static int buffer_pos;
	static int buffer_len;
	static int last_fd = -1;
	size_t line_pos = 0;
	char c;
	char *new_ptr;
	size_t new_size;

	if (lineptr == NULL || n == NULL)
		return (-1);

	if (last_fd != fd)
	{
		buffer_pos = 0;
		buffer_len = 0;
		last_fd = fd;
	}

	if (*lineptr == NULL || *n == 0)
	{
		*n = 128;
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}

	while (1)
	{
		if (buffer_pos >= buffer_len)
		{
			buffer_len = read(fd, buffer, BUFFER_SIZE);
			buffer_pos = 0;
			if (buffer_len <= 0)
			{
				if (line_pos == 0)
					return (-1);
				break;
			}
		}

		c = buffer[buffer_pos++];

		if (line_pos >= *n - 1)
		{
			new_size = *n * 2;
			new_ptr = malloc(new_size);
			if (new_ptr == NULL)
				return (-1);
			_memcpy(new_ptr, *lineptr, line_pos);
			free(*lineptr);
			*lineptr = new_ptr;
			*n = new_size;
		}

		(*lineptr)[line_pos++] = c;

		if (c == '\n')
			break;
	}

	(*lineptr)[line_pos] = '\0';
	return (line_pos);
}
