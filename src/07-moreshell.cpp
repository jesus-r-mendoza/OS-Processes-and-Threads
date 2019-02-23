#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char inp[25]; 
char *cmd[5];

int parseLine(char * line) {
char * p;
int count = 0;
p = strtok(line, " ");  // for splitting a string 
while (p && strcmp(p,"|") != 0) {
cmd[count] = p;
count++;
p = strtok(NULL," ");
}
}

static void prompt()
{
	printf("\nMoreShell > ");
	fgets(inp,25,stdin);
	inp[strlen(inp)-1]='\0';
	parseLine(inp);
}

int main(int argc,char **argv)
{
	prompt();

	pid_t ForkPID;

	while(strcmp(inp,"exit")!=0)
	{
		ForkPID = fork();
	switch (ForkPID) {
	case -1: printf("Error: Failed to fork.\n"); break;
	case 0:
		if (execvp(cmd[0],cmd) == -1) {
		printf("Error: running command: '%s'\n",cmd[0]);
		exit(0);
		}
	break;

	default:
		waitpid(ForkPID,NULL,0);
		prompt();
	}
	}


}