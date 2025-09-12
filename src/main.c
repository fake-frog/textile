#include "include.h"
#include <stdio.h>

int process(double delta_time) {
  clear_screen();

  //  printf("DELTA TIME: %f\r\n", delta_time);

  Textile textile = {0};

  Pattern pattern = {
      .needle = {1, 1, WORD}, // the terminal starts at 1,1 for some reason
      .name = "TEST PATTERN",
      .x = 30,
      .y = 20};

  register_patten(&textile, &pattern);

  pattern.needle.x = 1;
  pattern.needle.y = 1;

  window_size ws = get_window_size();

  char message[128];

  sprintf(message, "char x -> %d", ws.char_x);
  sow(message, &pattern);
  return_needle(&pattern.needle);

  sprintf(message, "char y -> %d", ws.char_y);
  sow(message, &pattern);
  return_needle(&pattern.needle);

  sprintf(message, "pixel x -> %d", ws.pixel_x);
  sow(message, &pattern);
  return_needle(&pattern.needle);

  move_needle(&pattern.needle, 10, 6);
  sprintf(message, "pixel y -> %d", ws.pixel_y);
  sow(message, &pattern);

  return_needle(&pattern.needle);
  sprintf(message, "pixel y -> %d", ws.pixel_y);
  sow(message, &pattern);

  return 0;
}

int main() {
  begin_textile(process);
  return 0;
}
