#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#define MAX_INPUT_SIZE 1024
#define PATH_MAX_LENGTH 4096

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
	char prompt[] = "Shell$ ";

	write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);
}

/**
 * concatenate_path - Concatenates the command with the path
 * @path: Pointer to the path
 * @input: Pointer to the input
 * @com_path: Pointer to the concatenated path
 */
void concatenate_path(char *path, char *input, char *com_path)
{
	int len = strlen(path), input_len = strlen(input);
	int i, j;

	for (i = 0; i < len; i++)
		com_path[i] = path[i];
	if (com_path[i - 1] != '/')
	{
		com_path[i] = '/';
		i++;
	}
	for (j = 0; j < input_len; j++, i++)
		com_path[i] = input[j];

	com_path[i] = '\0';
}


/**
 * handle_command - Handles command not found
 */
void handle_command(void)
{
	char error[] = "Shell: Command not found\n";

	write(STDERR_FILENO, error, sizeof(error) - 1);
}

/**
 * execute_command - Executes the command
 * @command_path: Pointer to command path
 * @command: Pointer to the command
 */
void execute_command(char *command_path, char *command)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		if (execl(command_path, command, NULL) == -1)
		{
			char error[] = "Shell: Execution failed\n";

			write(STDERR_FILENO, error, sizeof(error) - 1);
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		char err[] = "Shell: Fork failed\n";

		write(STDERR_FILENO, err, sizeof(err) - 1);
	}
	else
	{
		do

		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}


/**
 * find_and_execute - Function to find and execute command
 * @input: Pointer to the input
 * @path: Pointer to the path
 */
void find_and_execute(char *input, char *path)
{
	size_t len = strlen(input);
	int found = 0;
	char com_path[PATH_MAX_LENGTH];
	char *token = strtok(path, ":");

	if (len > 0 && input[len - 1] == '\n')
		input[len - 1] = '\0';

	while (token != NULL && !found)
	{
		concatenate_path(token, input, com_path);
		if (access(com_path, X_OK) == 0)
		{
			found = 1;
			break;
		}
		token = strtok(NULL, ":");
	}
	if (found)
		execute_command(com_path, input);
	else
		handle_command();
}
