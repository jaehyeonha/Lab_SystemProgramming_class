#include <stdio.h>
#include <unistd.h>
#include <curses.h>

int main()
{
    initscr();
    clear();

    move(10,20);
    addstr("Hello, world");
    move(LINES-1,0);
    refresh();
    sleep(1);
        
    move(10,20);
    addstr("            ");
    move(LINES-1,0);
    refresh();
    sleep(1);
    
    move(10,20);
    addstr("Hello, world");
    move(LINES-1,0);
    refresh();

    getch();
    endwin();

    return 0;
}