#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "shell.h"

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
