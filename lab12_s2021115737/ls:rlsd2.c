#include       <stdio.h>
#include       <stdlib.h>
#include       <unistd.h>
#include       <sys/types.h>
#include       <sys/socket.h>
#include       <netinet/in.h>
#include       <netdb.h>
#include       <time.h>
#include       <ctype.h>
#include       <string.h>

#define     oops(msg)       { perror(msg); exit(1); }
#define     PORTNUM     15000
#define     HOSTLEN     256
#define     READ        0
#define     WRITE       1

void sanitize(char *);

int main(int ac, char *av[])
{
       struct sockaddr_in  saddr;       
	struct hostent      *hp;  
       char   hostname[HOSTLEN];          
	int    sock_id, sock_fd;    
       FILE   *sock_fpi, *sock_fpo;
       FILE   *pipe_fp;
       char   dirname[BUFSIZ];     
	char   command[BUFSIZ];            
	int    dirlen, c;                        

	sock_id = socket( AF_INET, SOCK_STREAM, 0 );   
	if ( sock_id == -1 ) 
		oops( "socket" );          	

	bzero( (void *)&saddr, sizeof(saddr) );  
       gethostname(hostname, HOSTLEN);   
	hp = gethostbyname(hostname);            ;            	
	bcopy(hp->h_addr, (struct sockaddr *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);      
	saddr.sin_family = AF_INET ;         
	if ( bind(sock_id,(struct sockaddr *)&saddr, sizeof(saddr)) !=0)
	       oops( "bind" );

       if ( listen(sock_id, 1) != 0)
           oops("listen");
    
	while(1){
              sock_fd = accept(sock_id, NULL, NULL);
              if( sock_fd == -1)
                     oops("accept");

              if( (sock_fpi = fdopen(sock_fd,"r")) == NULL )
                     oops("fdopen reading");

              if( fgets(dirname, BUFSIZ-5, sock_fpi) == NULL)
                     oops("reading dirname");
                     sanitize(dirname);

              if( (sock_fpo = fdopen(sock_fd,"w")) == NULL)
                     oops("fdopen writing");

              sprintf(command, "ls %s", dirname);

              int pfp[2], pid;

              if(pipe(pfp)== -1)
                    oops("pipe")

              if((pid = fork()) == -1 ){
                     close(pfp[0]);
                     close(pfp[1]);
                     oops("fork");
              }

              if( pid > 0){
                     if(close(pfp[WRITE]) == -1)
                            oops("child pfp");
                     pipe_fp = fdopen(pfp[READ], "r");
                     while(( c = getc(pipe_fp)) != EOF)
                            putc(c, sock_fpo);
              }
              else{
                     if(close(pfp[READ])==-1)
                            oops("parent pfp");
                     if(dup2(pfp[WRITE],WRITE)==-1)
                            oops("dup");
                     if(close(pfp[WRITE])==-1)
                            oops("child pfp");
                     execlp("ls","ls",dirname,NULL);
                     oops("execlp ls");
              }
             
              fclose(pipe_fp);
              fclose(sock_fpo);
              fclose(sock_fpi);
       }
	return 0;
}

void sanitize(char *str)
{
    char *src, *dest;

    for(src = dest = str; *src; src++)
        if (*src == '/' || isalnum(*src))
            *dest++ = *src;

    *dest = '\0';
}