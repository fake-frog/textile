#include "include.h"

struct termios orig_termios;

// Move cursor to row, col (1-indexed)
void move_cursor(int row, int col) {
  // negative number set bolth x and y to 0
  printf("\033[%d;%dH", row, col);
  fflush(stdout);
}

// Clear screen and move to top-left
void clear_screen() {
  printf("\033[2J\033[H");
  fflush(stdout);
}

void disable_raw_mode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  // Non-blocking read with timeout
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
