#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int main() {
    int max_y = 0, max_x = 0;
    int balloon_x = 10, balloon_y = 10;
    int dir_x = 1, dir_y = 1;
    int paddle_y = 10;
    int ch;

    initscr();            // Start ncurses mode
    noecho();             // Don’t echo pressed keys
    curs_set(FALSE);      // Hide cursor
    keypad(stdscr, TRUE); // Enable arrow keys
    nodelay(stdscr, TRUE);// Non-blocking input

    getmaxyx(stdscr, max_y, max_x); // Get screen size

    while (1) {
        clear();

        // Draw balloon (instead of ball)
        mvprintw(balloon_y, balloon_x, "🎈");

        // Draw paddle (right side)
        for (int i = 0; i < 4; i++) {
            mvprintw(paddle_y + i, max_x - 2, "|");
        }

        refresh();

        // Move balloon
        balloon_x += dir_x;
        balloon_y += dir_y;

        // Bounce on walls
        if (balloon_x <= 0 || balloon_x >= max_x - 2) dir_x *= -1;
        if (balloon_y <= 0 || balloon_y >= max_y - 1) dir_y *= -1;

        // Collision with paddle
        if (balloon_x >= max_x - 3 &&
            balloon_y >= paddle_y &&
            balloon_y <= paddle_y + 3) {
            dir_x *= -1;
        }

        // Game over (balloon goes past paddle)
        if (balloon_x >= max_x - 1) {
            clear();
            mvprintw(max_y/2, max_x/2 - 6, "🎈 GAME OVER 🎈");
            refresh();
            sleep(2);
            break;
        }

        // Input for paddle movement
        ch = getch();
        if (ch == KEY_UP && paddle_y > 0) paddle_y--;
        if (ch == KEY_DOWN && paddle_y < max_y - 4) paddle_y++;

        usleep(DELAY);
    }

    endwin(); // End ncurses
    return 0;
}
