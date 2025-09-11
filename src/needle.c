#include "include.h"

// maybe we will append the sequence to the
// pattern then printf to keep track of the
// full sequence on screen?

// we will probably need to set the x and y
// relative to the patterns position

void sow_point(Needle *needle, char *string) {
  // append to pattern here
  printf("%s", string);
  move_cursor(needle->x, needle->y);
}

void sow_char(Needle *needle, char *string) {
  // append to pattern here
  printf("%s", string);
  move_cursor(++needle->x, needle->y);
}

void sow_word(Needle *needle, char *string) {
  printf("%s", string);
  move_cursor(needle->x += (strlen(string) + 1), needle->y);
}

void sow_line(Needle *needle, char *string) {
  // move to new line if we are not at the begining
  if (needle->x > 1)
    move_cursor(1, ++needle->y);
  printf("%s", string);
  move_cursor(1, ++needle->y);
}

// this onese a bit more complicated
// need to search for certain symbole
// in the squence
void find_mark(Needle *needle) {}

void sow_mark(Needle needle, char *string) {}
