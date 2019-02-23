#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>



char inp[25];
char *cmd1[5];
char *cmd2[5];
int f;
int mypipe[2];

int parseline(char * line , char * cmd[]) {          
char * p;
int count = 0;
p = strtok(line, " ");          
while (p && strcmp(p,"|") != 0) {
cmd[count] = p;
count++;
p = strtok(NULL," ");
}
cmd[count]=NULL;
return count;
}

static void prompt()                     //command line 
{
	printf("\nDupShell > "); 
	fgets(inp,25,stdin);         //read command line 
	inp[strlen(inp)-1]='\0';
	int count =0;
	char line1[5];

	if(strchr(inp,'|')!=NULL)    //strchr(const char *str, int c) searches for the first occurrence of the character c (an unsigned char)

	{
	f=1;
	char *t[2];
	char *p;
	p = strtok(inp, "|");    //strtok() divides the string into tokens. "|"  
	while (p) {
	t[count] = p;
	count++;
	p = strtok(NULL,"|");
	}
	parseline(t[0],cmd1);
	parseline(t[1],cmd2);
	}

	else
	{
		f=0;
		parseline(inp,cmd1);
	}
}


int main(int argc,char **argv)
{
	prompt();

	pid_t ForkPID,ForkPID1;

	while(strcmp(inp,"exit")!=0)
	{
		if (pipe(mypipe)) {
					printf("\n Sorry, Your pipe Failed \n");
					return (0);
				}


		ForkPID = fork();
	switch (ForkPID) {
	case -1: printf("Error: Failed to fork.\n"); break;
	case 0:

			if(f==1)
			{
			dup2(mypipe[1],STDOUT_FILENO);//this command redirects the output of the executed command into the pipe
			close(mypipe[0]);
			close(mypipe[1]);
			}
			if (execvp(cmd1[0],cmd1) == -1) {
		printf("Error: running command: '%s'\n",cmd1[0]);
		exit(0);
		}
			exit(0);


	break;

	default:
		ForkPID1 = fork();
		switch (ForkPID1) {
			case -1: printf("Error: Failed to fork.\n"); break;
			case 0:
				dup2(mypipe[0],STDIN_FILENO); //this command reads from the pipe and execvp executes it
				close(mypipe[0]);
				close(mypipe[1]);
				if(f==1)
				{
				if (execvp(cmd2[0],cmd2) == -1) {   
				printf("Error: running command: '%s'\n",cmd2[0]);
				exit(0);
				}
				}
				exit(0);
			break;

			default:
				close(mypipe[0]);
				close(mypipe[1]);
				waitpid(ForkPID,NULL,0);
				waitpid(ForkPID1,NULL,0);
				prompt();
			}
	}
	}


}

