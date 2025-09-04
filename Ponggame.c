#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int main() {
    int max_y = 0, max_x = 0;
    int ball_x = 10, ball_y = 10;
    int dir_x = 1, dir_y = 1;
    int paddle_y = 10;
    int ch;

    initscr();            // Start ncurses mode
    noecho();             // Don’t echo pressed keys
    curs_set(FALSE);      // Hide cursor
    keypad(stdscr, TRUE); // Enable arrow keys
    nodelay(stdscr, TRUE);// Non-blocking input

    getmaxyx(stdscr, max_y, max_x); // Screen size

    while (1) {
        clear();

        // Draw ball
        mvprintw(ball_y, ball_x, "O");

        // Draw paddle (right side)
        for (int i = 0; i < 4; i++) {
            mvprintw(paddle_y + i, max_x - 2, "|");
        }

        refresh();

        // Ball movement
        ball_x += dir_x;
        ball_y += dir_y;

        // Collision with walls
        if (ball_x <= 0 || ball_x >= max_x - 1) dir_x *= -1;
        if (ball_y <= 0 || ball_y >= max_y - 1) dir_y *= -1;

        // Paddle collision
        if (ball_x >= max_x - 3 &&
            ball_y >= paddle_y &&
            ball_y <= paddle_y + 3) {
            dir_x *= -1;
        }

        // Game over (ball passes paddle)
        if (ball_x >= max_x - 1) {
            clear();
            mvprintw(max_y/2, max_x/2 - 5, "GAME OVER!");
            refresh();
            sleep(2);
            break;
        }

        // Input for paddle
        ch = getch();
        if (ch == KEY_UP && paddle_y > 0) paddle_y--;
        if (ch == KEY_DOWN && paddle_y < max_y - 4) paddle_y++;

        usleep(DELAY);
    }

    endwin(); // End ncurses
    return 0;
}
