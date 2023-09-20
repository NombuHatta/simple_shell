#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

#define MAX_INPUT_SIZE 1024

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
	char prompt[] = "Shell$ ";

	write(STDOUT_FILENO, prompt, sizeof(prompt) - 1);
}

/**
 * exec_command - Execute a shell command
 * @command: The command to be executed.
 */
void exec_command(char *command)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		char *args[2];

		args[0] = command;
		args[1] = NULL;

		if (execvp(command, args) == -1)
		{
			char error[] = "simple_shell: Command not found\n";

			write(STDERR_FILENO, error, sizeof(error) - 1);
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		char err[] = "simple_shell: Fork failed\n";

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
 * execute_command - Executes a given command.
 * @input: The input string containing the command.
 */
void execute_command(char *input)
{
	char *token = strtok(input, " ");
	char *command = token, *args[MAX_INPUT_SIZE];
	int arg_count = 0, status;
	pid_t pid;

	if (command == NULL)
		return;
	while (token != NULL)
	{
		args[arg_count] = token;
		arg_count++;
		token = strtok(NULL, " ");
	}
	args[arg_count] = NULL;

	pid = fork();
	if (pid == 0)
	{
		if (execvp(command, args) == -1)
		{
			char error[] = "Shell: Command not found\n";

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
