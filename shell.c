/*
 * shell.c
 *
 *  Created on: Feb 18, 2018
 *      Author: hunter
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLINE 80 /*command line with max 40 arguments */

int main(void)
{
	char *args[MAXLINE/2 + 1]; // command line with max 40 args
	int should_run = 1; // flag to determine when to stop program
	printf("CS149 Shell from Hunter Lai\n");
	char exit[] = "exit";
	char ampersand[] = "&";
	size_t max = MAXLINE;

	while(should_run)
	{
		printf("hunter-049>");
		fflush(stdout);
		char *command = malloc((MAXLINE/2 + 1)*sizeof(char));
		getline(&command, &max, stdin); // retrieves input from user

		if(strlen(command) > (MAXLINE + 1)) // checks to see if input is too long
		{
			printf("Too long of a command, please try a new command.\n");
			continue;
		}

		int counter = 1; // begin parsing the input
		args[0] = strtok(command, " \n"); // parses input by spaces and newlines
		int waiting = 1;
		while(args[counter-1] != NULL)
		{
			args[counter] = strtok(NULL, " \n");
			counter++;
		}

		if(strcmp(args[0], exit) == 0) // checks to see if the input requests to exit
		{
			should_run = 0;
			break;
		}

		if(strcmp(args[counter-2], ampersand) == 0) // checks to see if ampersand at end of input
		{
			waiting = 0;
			args[counter-2] = NULL;
		}

		pid_t pid = fork(); // process forks
		if(pid == 0)//Child process
		{
			execvp(args[0], args); // executes process
			printf("Failed execution, please try a new command.\n"); // output if execution fails
			should_run = 0;
		}
		else if(pid > 0)//Parent process
		{
			if(waiting == 1) // if there is no ampersand, then wait
				wait(NULL);
		}
		else // fork fails
			printf("Failed forking, please try again.\n");
		free(command);
	}
	return 0;
}
