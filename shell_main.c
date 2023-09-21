#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "main.h"

#define MAX_INPUT_SIZE 1024

/**
 * main - Entry point of the shell program
 * Return: Always 0 (Success)
 */
int main(void)
{
	char input[MAX_INPUT_SIZE];
	char *path = getenv("PATH");

	while (1)
	{
		display_prompt();

		if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}

		find_and_execute(input, path);
	}
	return (0);
}
