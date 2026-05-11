#include "main.h"

/**
 * main - Entry point for the simple shell
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "($) ", 4);

		read = getline(&line, &len, stdin);

		if (read == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			exit(EXIT_SUCCESS);
		}

		if (line[read - 1] == '\n')
			line[read - 1] = '\0';

		/* Simple check for empty lines */
		if (strlen(line) > 0)
		{
			/* For now, we just print the command or exit */
			if (strcmp(line, "exit") == 0)
				break;
		}
	}
	free(line);
	return (0);
}
