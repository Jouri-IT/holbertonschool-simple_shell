# Simple Shell

A simple UNIX command line interpreter written in C, built as part of the Holberton School curriculum by Aseel Alzhrani and Jouri Alsulaiman.

## Description

`hsh` is a custom shell that replicates the core behavior of `/bin/sh`. It reads commands from standard input, resolves them using the PATH environment variable, and executes them via `fork` and `execve`. It supports both interactive and non-interactive modes.

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage

### Interactive mode

```bash
$ ./hsh
($) /bin/ls
hsh main.c shell.c
($) exit
$
```

### Non-interactive mode

```bash
$ echo "/bin/ls" | ./hsh
hsh main.c shell.c
```

## Built-in Commands

| Command | Description |
|---------|-------------|
| `exit` | Exit the shell with the last command's exit status |
| `env` | Print all current environment variables |

## How It Works

1. Prints a `($) ` prompt in interactive mode
2. Reads a line of input using `getline`
3. Tokenizes the input into command and arguments using `strtok`
4. Resolves the command's full path by searching directories in `PATH`
5. Forks a child process and executes the command with `execve`
6. Waits for the child to finish and stores the exit status
7. Repeats until `exit` or EOF (Ctrl+D)

## Files

| File | Description |
|------|-------------|
| `shell.c` | Main shell loop, built-ins, and command execution |
| `_getenv.c` | Custom `getenv` — searches `environ` for a variable by name |
| `path_handler.c` | Resolves full command path by searching PATH directories |
| `main.h` | Header file with function prototypes and includes |
| `man_1_simple_shell` | Manual page for hsh |
| `AUTHORS` | List of project contributors |

## Error Handling

Errors match `/bin/sh` format:

```bash
$ echo "qwerty" | ./hsh
./hsh: 1: qwerty: not found
```

## Requirements

- Ubuntu 20.04 LTS
- gcc with flags: `-Wall -Werror -Wextra -pedantic -std=gnu89`
- Betty style compliant
- No memory leaks
- No more than 5 functions per file

## Authors

- Aseel Alzhrani
- Jouri Alsulaiman
