#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include <string.h>

/**
* main - Entry point for the shell
*
* Return: exit status of last executed command
*/
int main(void)
{
char *line, *cmd;
int last_status = 0;

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

/* Built-in exit */
if (strcmp(cmd, "exit") == 0)
{
free(line);
exit(last_status); /* exit shell with last status */
}

if (cmd[0] != '\0')
last_status = execute_command(cmd);

free(line);
}

return (last_status);
}
