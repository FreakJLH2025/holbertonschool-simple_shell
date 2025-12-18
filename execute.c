#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
* execute_command - Forks and executes a command with arguments via execve
* @line: Command line to execute
*
* Return: Nothing
*/
void execute_command(char *line)
{
pid_t pid = fork();
int status;

if (pid == -1)
{
perror("fork");
return;
}
if (pid == 0)
{
char **argv = split_line(line);

if (execve(argv[0], argv, environ) == -1)
{
perror("./shell");
free(argv);
exit(EXIT_FAILURE);
}
free(argv);
}
else
{
waitpid(pid, &status, 0);
}
}
