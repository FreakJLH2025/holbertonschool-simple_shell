#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "shell.h"

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
* read_line - Reads one line from stdin and strips trailing newline
*
* Return: Pointer to the allocated line, or NULL on EOF/error
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
if (read > 0 && line[read - 1] == '\n')
line[read - 1] = '\0';
return (line);
}

/**
* trim_spaces - Removes leading and trailing spaces/tabs
* @str: The string to trim
*
* Return: Pointer to the trimmed string (same buffer)
*/
char *trim_spaces(char *str)
{
char *end;

while (*str == ' ' || *str == '\t')
str++;

if (*str == '\0')
return (str);

end = str + strlen(str) - 1;
while (end > str && (*end == ' ' || *end == '\t'))
end--;

*(end + 1) = '\0';
return (str);
}

/**
* split_line - Splits a line into tokens separated by spaces/tabs
* @line: The input line
*
* Return: NULL-terminated array of tokens (must be freed)
*/
char **split_line(char *line)
{
char **tokens = NULL;
char *token;
size_t bufsize = 64, i = 0;

tokens = malloc(bufsize * sizeof(char *));
if (!tokens)
{
perror("malloc");
exit(EXIT_FAILURE);
}

token = strtok(line, " \t");
while (token != NULL)
{
tokens[i++] = token;
if (i >= bufsize)
{
bufsize += 64;
tokens = realloc(tokens, bufsize *sizeof(char *));
if (!tokens)
{
perror("realloc");
exit(EXIT_FAILURE);
}
}
token = strtok(NULL, " \t");
}
tokens[i] = NULL;
return (tokens);
}

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

/**
* main - Entry point for the simple UNIX command line interpreter
*
* Return: Always 0
*/
int main(void)
{
char *line, *cmd;

while (1)
{
if (isatty(STDIN_FILENO))
display_prompt();

line = read_line();
if (line == NULL)
{
if (isatty(STDIN_FILENO))
printf("\n");
break;
}

cmd = trim_spaces(line);
if (cmd[0] != '\0')   /* ignore empty or spaces-only lines */
execute_command(cmd);

free(line);
}
return (0);
}
