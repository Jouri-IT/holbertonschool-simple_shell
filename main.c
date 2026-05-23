#include "shell.h"

/**
 * main - Entry point for the simple shell
 * @argc: argument count
 * @argv: argument vector
 * Return: exit status
 */
int main(int argc, char **argv)
{
	int fd = STDIN_FILENO;
	int status;

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			fprintf(stderr, "%s: 0: Can't open %s\n",
				argv[0], argv[1]);
			exit(127);
		}
	}
	status = shell_loop(fd, argv);
	if (fd != STDIN_FILENO)
		close(fd);
	return (status);
}
