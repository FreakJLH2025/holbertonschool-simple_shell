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
if (!path || *path == '\0')
{
free(path); /* free if empty or NULL to avoid leak */
return (NULL);
}

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
* execute_command - Executes a command line. No fork if command not found.
* @line: Command line to execute
*
* Return: exit status for the command (127 if not found, child status otherwise)
*/
int execute_command(char *line)
{
char **argv = split_line(line);
char *cmd_path = NULL;
pid_t pid;
int status = 0;

if (!argv || !argv[0])
{
free(argv);
return (0);
}

/* Determine command path: direct (contains '/') or via PATH lookup */
if (strchr(argv[0], '/'))
cmd_path = strdup(argv[0]);
else
cmd_path = find_command(argv[0]);

if (!cmd_path)
{
/* Required error format and status when command not found */
fprintf(stderr, "./hsh: 1: %s: not found\n", argv[0]);
free(argv);
return (127); /* no fork, return 127 to caller */
}

pid = fork();
if (pid == -1)
{
perror("fork");
free(argv);
free(cmd_path);
return (1);
}

if (pid == 0)
{
/* Child: execute command with full argv */
if (execve(cmd_path, argv, environ) == -1)
{
perror("./hsh");
free(argv);
free(cmd_path);
exit(EXIT_FAILURE);
}
}
else
{
/* Parent: wait for child and propagate its exit status */
if (waitpid(pid, &status, 0) == -1)
status = 1;
else if (WIFEXITED(status))
status = WEXITSTATUS(status);
else
status = 1;
}

free(argv);
free(cmd_path);
return (status);
}
