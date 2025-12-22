#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

/**
* get_path - Retrieve PATH from environ without using getenv
*
* Return: malloc'd string with PATH value, or NULL if not found
*/
char *get_path(void)
{
int i = 0;
char *path = NULL;

while (environ[i])
{
if (strncmp(environ[i], "PATH=", 5) == 0)
{
path = strdup(environ[i] + 5); /* skip "PATH=" */
return (path);
}
i++;
}
return (NULL);
}

/**
* find_command - Search for a command in PATH
* @command: The command name
*
* Return: Full path if found (malloc'd), NULL if not
*/
char *find_command(char *command)
{
char *path, *dir, *full_path;
size_t len;

path = get_path();
if (!path)
return (NULL);

dir = strtok(path, ":");
while (dir != NULL)
{
len = strlen(dir) + strlen(command) + 2;
full_path = malloc(len);
if (!full_path)
{
free(path);
return (NULL);
}
snprintf(full_path, len, "%s/%s", dir, command);
if (access(full_path, X_OK) == 0)
{
free(path);
return (full_path); /* caller must free */
}
free(full_path);
dir = strtok(NULL, ":");
}
free(path);
return (NULL);
}

/**
* execute_command - Forks and executes a command with arguments via execve
* @line: Command line to execute
*
* Return: Nothing
*/
void execute_command(char *line)
{
char **argv = split_line(line);
char *cmd_path;
pid_t pid;
int status;

if (!argv || !argv[0])
{
free(argv);
return;
}

/* PATH lookup */
if (strchr(argv[0], '/'))
cmd_path = strdup(argv[0]);
else
cmd_path = find_command(argv[0]);

if (!cmd_path)
{
fprintf(stderr, "./shell: command not found: %s\n", argv[0]);
free(argv);
return; /* no fork */
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(argv);
free(cmd_path);
return;
}
if (pid == 0)
{
/* Pass full argv (with arguments) */
if (execve(cmd_path, argv, environ) == -1)
{
perror("./shell");
free(argv);
free(cmd_path);
exit(EXIT_FAILURE);
}
}
else
{
waitpid(pid, &status, 0);
}
free(argv);
free(cmd_path);
}
