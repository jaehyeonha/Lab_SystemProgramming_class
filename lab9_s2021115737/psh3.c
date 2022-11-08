#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define	MAXARGS	20				
#define	ARGLEN	100		

void execute (char **);
char * makestring(char *);
void sigint_handler(int);

int main()
{
	signal(SIGINT, sigint_handler);
    char *arglist[MAXARGS+1];		
	int	numargs;			
	char argbuf[ARGLEN];			
	char *makestring();			

	numargs = 0;
	while ( numargs < MAXARGS )
	{			
        if(feof(stdin))
            exit(0);		
		printf("Arg[%d]? ", numargs);
		if ( fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n' )
			arglist[numargs++] = makestring(argbuf);
            if(strcmp(argbuf, "exit") == 0)
                exit(0);
		else
		{
			if ( numargs > 0 ){		
				arglist[numargs]=NULL;	
				execute( arglist );
				numargs = 0;		
			}
		}
	}
	return 0;
}

void execute( char *arglist[] )
{
	int	pid,exitstatus;				

	pid = fork();					
	switch( pid ){
		case -1:	
			perror("fork failed");
			exit(1);
		case 0:
			execvp(arglist[0], arglist);		
			perror("execvp failed");
			exit(1);
		default:
			while( wait(&exitstatus) != pid )
				;
			printf("child exited with status %d,%d\n",
					exitstatus>>8, exitstatus&0377);
	}
}
char *makestring( char *buf )
{
	char	*cp;

	buf[strlen(buf)-1] = '\0';		
	cp = malloc( strlen(buf)+1 );		
	if ( cp == NULL ){			
		fprintf(stderr,"no memory\n");
		exit(1);
	}
	strcpy(cp, buf);		
	return cp;			
}

void sigint_handler(int sig)
{

}
