#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

static struct termios orig_termios;

static void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

static void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(reset_terminal);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

static void draw_board(void) {
    int fd = open("/proc/factorio/board", O_RDONLY);
    if (fd < 0) return;
    char buf[1024];
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
    }
    close(fd);
}

int main(void) {
    int cmd_fd = open("/proc/factorio/cmd", O_WRONLY);
    if (cmd_fd < 0) {
        perror("Failed to open /proc/factorio/cmd");
        return 1;
    }

    enable_raw_mode();
    draw_board();

    char ch;
    while (read(STDIN_FILENO, &ch, 1) == 1) {
        if (ch == 'q') break;
        write(cmd_fd, &ch, 1);
        draw_board();
    }

    close(cmd_fd);
    return 0;
}