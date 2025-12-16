#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/**
* display_prompt - Prints the shell prompt
*
* Return: Nothing
*/
void display_prompt(void)
{
printf("#cisfun$ ");
fflush(stdout);
}

/**
* read_line - Reads a line from stdin
*
* Return: Pointer to the line read, or NULL on EOF
*/
char *read_line(void)
{
char *line = NULL;
size_t len = 0;
ssize_t read = getline(&line, &len, stdin);

if (read == -1)
{
free(line);
return (NULL);
}
line[read - 1] = '\0';
return (line);
}

/**
* execute_command - Executes a command using execve
* @line: Command to execute
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
char *argv[2] = {line, NULL};
if (execve(argv[0], argv, environ) == -1)
{
perror("./shell");
exit(EXIT_FAILURE);
}
}
else
waitpid(pid, &status, 0);
}

/**
* main - Entry point of the simple shell
*
* Return: Always 0
*/
int main(void)
{
char *line;

while (1)
{
display_prompt();
line = read_line();
if (line == NULL)
{
printf("\n");
break;
}
if (strlen(line) > 0)
execute_command(line);
free(line);
}
return (0);
}
