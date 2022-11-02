#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <termios.h>


int main() {
    struct termios info, new_info;
    char input;

    initscr();
    clear();

    tcgetattr(STDIN_FILENO, &info);
    new_info = info;
    new_info.c_lflag &= ~ICANON;
    new_info.c_lflag &= ~ECHO;
    new_info.c_cc[VMIN] = 0;
    new_info.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_info);

    while(1) {
        if(read(STDIN_FILENO, &input, 1) > 0)
            break;

        move(10, 20);
        addstr("Hellow, world");
        move(LINES-1, 0);
        refresh();

        sleep(1);

        move(10, 20);
        addstr("             ");
        move(LINES-1, 0);
        refresh();

        sleep(1);
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &info);
    endwin();

    return 0;
}