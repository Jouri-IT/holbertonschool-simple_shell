# simple shell

a simple unix command line interpreter written in c, built as part of the holberton school curriculum by aseel alzhrani and jouri alsulaiman.

---

## description

`hsh` is a custom shell that replicates the core behavior of `/bin/sh`. it reads commands from standard input, resolves them using the path environment variable, and executes them via `fork` and `execve`. it supports both interactive and non-interactive modes.

---

## compilation

gcc -wall -werror -wextra -pedantic -std=gnu89 *.c -o hsh

---

## usage

### interactive mode

$ ./hsh  
($) /bin/ls  
hsh main.c shell.c  
($) exit  
$

### non-interactive mode

$ echo "/bin/ls" | ./hsh  
hsh main.c shell.c  

---

## built-in commands

| command | description |
|---------|-------------|
| exit | exit the shell with the last command's exit status |
| env | print all current environment variables |

---

## how it works

1. prints a ($) prompt in interactive mode  
2. reads input using getline  
3. tokenizes input using strtok  
4. resolves command path using path environment variable  
5. forks a child process  
6. executes command using execve  
7. waits for process to finish and stores exit status  
8. repeats until exit or ctrl+d  

---

## files

| file | description |
|------|-------------|
| shell.c | main shell loop, built-ins, and execution logic |
| _getenv.c | custom getenv implementation |
| path_handler.c | resolves command paths using path variable |
| main.h | header file with function prototypes |
| man_1_simple_shell | manual page for hsh |
| authors | project contributors list |

---

## error handling

errors match /bin/sh format:

$ echo "qwerty" | ./hsh  
./hsh: 1: qwerty: not found  

---

## requirements

- ubuntu 20.04 lts  
- gcc with flags: -wall -werror -wextra -pedantic -std=gnu89  
- betty style compliant  
- no memory leaks  
- no more than 5 functions per file  

---

## authors

- aseel alzhrani  
- jouri alsulaiman  
