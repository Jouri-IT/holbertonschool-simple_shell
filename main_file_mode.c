#include "shell.h"

/**
 * main - Entry point for the simple shell
 * @argc: Argument count
 * @argv: Argument vector
 * 
 * Return: 0 on success, or 127 on file error
 */
int main(int argc, char **argv)
{
	int fd = STDIN_FILENO;

	/* 1. Check if a filename was passed as an argument */
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			/* Exact error format expected by the checker */
			fprintf(stderr, "%s: 0: Can't open %s\n", argv[0], argv[1]);
			exit(127);
		}
	}

	/* 2. Execute your shell loop */
	/* 
	 * Modify your loop function to accept 'fd' so it knows whether to call
	 * read_from_file(fd) or your regular stdin getline function.
	 * 
	 * Example: shell_loop(fd, argv);
	 */

	/* 3. Close the file descriptor if it was opened */
	if (fd != STDIN_FILENO)
		close(fd);

	return (0);
}
