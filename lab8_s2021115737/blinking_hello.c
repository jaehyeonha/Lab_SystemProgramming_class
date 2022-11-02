#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>

#define	MESSAGE	"hello world"
#define BLANK   "           "

void move_msg(int);
int set_ticker(int);

int main()
{
	void	move_msg(int);	
    int	delay;

	initscr();
	crmode();
	noecho();
	clear();
		
    delay=2000;

	move(10,20);		
	addstr(MESSAGE);
    refresh();		
	signal(SIGALRM, move_msg );
	set_ticker( delay );

	while(1)
	{
		getch();
		break;
	}
	endwin();
	return 0;
}

void move_msg(int signum)
{	
    move( 10, 20 );
	addstr( BLANK );
    refresh();	
   
    sleep(1);

	move( 10, 20 );		
	addstr( MESSAGE );		
	refresh();	  
}

int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec,n_usecs;

    n_sec=n_msecs/1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;

    return setitimer(ITIMER_REAL,&new_timeset,NULL);
}