#ifndef MAIN_H
#define MAIN_H

void display_prompt(void);
void find_and_execute(char *input, char *path);
void execute_command(char *command_path, char *command);
void handle_command(void);
void concatenate_path(char *path, char *input, char *com_path);

#endif
