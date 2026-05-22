#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

/**
 * _getline - Reads an entire line from standard input using a static buffer.
 * @lineptr: Pointer to the buffer storing the read line.
 * @n: Pointer to the allocated size of lineptr.
 *
 * Return: Number of characters read (including newline), or -1 on failure/EOF.
 */
ssize_t _getline(char **lineptr, size_t *n)
{
	static char buffer[BUFFER_SIZE];
	static size_t buffer_pos;
	static size_t buffer_len;
	size_t line_pos = 0;
	char c;

	/* Validate input pointers */
	if (lineptr == NULL || n == NULL)
		return (-1);

	/* If lineptr is empty, allocate an initial buffer */
	if (*lineptr == NULL || *n == 0)
	{
		*n = 128; /* Start with a reasonable initial allocation */
		*lineptr = malloc(*n);
		if (*lineptr == NULL)
			return (-1);
	}

	while (1)
	{
		/* If the static buffer is empty, refill it using a single read call */
		if (buffer_pos >= buffer_len)
		{
			buffer_len = read(STDIN_FILENO, buffer, BUFFER_SIZE);
			buffer_pos = 0;

			/* End of file (EOF) or error occurred */
			if (buffer_len <= 0)
			{
				if (line_pos == 0)
					return (-1); /* Nothing read at all */
				break; /* Return what we managed to read so far */
			}
		}

		/* Grab the next character from our static buffer */
		c = buffer[buffer_pos++];

		/* Make sure lineptr has enough room, reallocate if necessary */
		if (line_pos >= *n - 1)
		{
			*n *= 2;
			*lineptr = realloc(*lineptr, *n);
			if (*lineptr == NULL)
				return (-1);
		}

		/* Store the character */
		(*lineptr)[line_pos++] = c;

		/* Stop processing if we hit a newline character */
		if (c == '\n')
			break;
	}

	/* Null-terminate the string */
	(*lineptr)[line_pos] = '\0';
	return (line_pos);
}
