#include	<stdio.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <fcntl.h>
#include    <sys/wait.h>

int main()
{
	int	pid ;
	int	fd;

	printf("About to run who into a file\n");

	if( (pid = fork() ) == -1 ){
		perror("fork"); exit(1);
	}

	if ( pid == 0 ){
		close(1);				
		fd = open( "userlist", O_CREAT|O_WRONLY|O_APPEND, 0644);	
		execlp( "who", "who", NULL );		
		perror("execlp");
		exit(1);
	}

	if ( pid != 0 ){
		wait(NULL);
		printf("Done running who.  results in userlist\n");
	}
}