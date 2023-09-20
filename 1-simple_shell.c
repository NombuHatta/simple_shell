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

/**
 * main - Entry point of the shell program
 * Return: Always 0 (Success)
 */
int main(void)
{
	char input[MAX_INPUT_SIZE];
	size_t len = strlen(input);

	while (1)
	{
		display_prompt();

		if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		if (len > 0 && input[len - 1] == '\n')
		{
			input[len - 1] = '\0';
		}

		if (strlen(input) == 0)
		{
			continue;
		}

		execute_command(input);
	}
	return (0);
}
