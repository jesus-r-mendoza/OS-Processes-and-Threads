#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char inp[25];
char *cmd[5];

int parseLine(char *line)              // input that users inputs through the minishell   line 
{
	char *p;
	int count = 0;
	p = strtok(line, " ");
	while (p && strcmp(p, "|") != 0)
	{
		cmd[count] = p;                        //parses the input stores in cmd []
		count++;
		p = strtok(NULL, " ");
	}
}

static void prompt()
{
	printf("\nMiniShell > ");
	fgets(inp, 25, stdin);
	inp[strlen(inp) - 1] = '\0';         //computes the length of the string str up to  
	parseLine(inp);
}

int main(int argc, char **argv)                   // main function forks a child 
{                                                 // forks a child everytime a command given to execute
	prompt();

	pid_t ForkPID;                    

	while (strcmp(inp, "exit") != 0)                  //string compare 
	{
		ForkPID = fork();                                           
		switch (ForkPID)
		{
		case -1:
			printf("Error: Failed to fork.\n");
			break;
		case 0:
			if (execlp(cmd[0], cmd[0], NULL) == -1)               //executes the argument shell command through execlp in the child.
			{
				printf("Error: running command: '%s'\n Please check your command \n\n", cmd[0]);
				exit(0);       //exits of shell if given command 'exit'. 
			}
			break;

		default:
			waitpid(ForkPID, NULL, 0);
			prompt();
		}
	}
}
