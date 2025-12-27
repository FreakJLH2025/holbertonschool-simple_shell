#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

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
