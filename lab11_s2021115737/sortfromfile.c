#include	<stdio.h>
#include    <unistd.h>
#include    <stdlib.h>
#include    <fcntl.h>
#include    <sys/wait.h>

int main(int ac, char* av[])
{
	int	pid ;
	int	fd;

	printf("About to run with input fron %s\n",av[1]);

	if( (pid = fork() ) == -1 ){
		perror("fork"); exit(1);
	}

	if ( pid == 0 ){	
		fd= open(av[1], O_RDONLY, 0644);		
		dup2(fd, 0);
		close(fd);	
		execlp( "sort", "sort", NULL );		
		perror("execlp");
		exit(1);
	}

	if ( pid != 0 ){
		wait(NULL);
		printf("Done running sort < %s\n",av[1]);
	}
}