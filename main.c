#include "shell.h"

/**
 * main - Entry point for the simple shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, or exit status
 */
int main(int argc, char **argv)
{
	int fd = STDIN_FILENO;

	/* 1. Check if a filename was passed as an argument */
	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		
		/* 2. If the file doesn't exist or lacks permissions */
		if (fd == -1)
		{
			fprintf(stderr, "%s: 0: Can't open %s\n", argv[0], argv[1]);
			exit(127);
		}
	}

	/* 3. Proceed to your shell loop */
	/* Make sure your loop function takes fd so it can read from the file! */
	shell_loop(fd, argv);

	if (fd != STDIN_FILENO)
		close(fd);

	return (0);
}
