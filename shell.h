/* shell.h */
#ifndef SHELL_H
#define SHELL_H

extern char **environ;

void display_prompt(void);
char *read_line(void);
void execute_command(char *line);
char *trim_spaces(char *str);
char **split_line(char *line);
#endif /* SHELL_H */
