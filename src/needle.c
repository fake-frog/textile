#include "include.h"

// maybe we will append the sequence to the
// pattern then printf to keep track of the
// full sequence on screen?

// we will probably need to set the x and y
// relative to the patterns position

void sow_point(Needle *needle, char *string) { printf("%s", string); }

void sow_char(Needle *needle, char *string) {
  printf("%s", string);
  needle->x++;
}

void sow_word(Needle *needle, char *string) {
  printf("%s", string);
  needle->x += (strlen(string) + 1);
}

void move_needle(Needle *needle, int x, int y) {
  needle->x = x;
  needle->y = y;
}

void return_needle(Needle *needle) {
  needle->x = 1;
  needle->y++;
}
