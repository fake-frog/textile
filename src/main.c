#include "include.h"

int process(double delta_time, Textile *textile) {
  clear_screen();

  //  printf("DELTA TIME: %f\r\n", delta_time);

  window_size ws = get_window_size();

  char message[128];

  Pattern *pattern = get_pattern(textile, "TEST PATTERN"); // can return null

  if (!pattern) {
    return 1;
  }
  reset_needle(&pattern->needle);

  sprintf(message, "char x -> %d", ws.char_x);
  sow(message, pattern);
  return_needle(&pattern->needle);

  sprintf(message, "char y -> %d", ws.char_y);
  sow(message, pattern);
  return_needle(&pattern->needle);

  sprintf(message, "pixel x -> %d", ws.pixel_x);
  sow(message, pattern);
  return_needle(&pattern->needle);

  sprintf(message, "pixel y -> %d", ws.pixel_y);
  sow(message, pattern);

  return 0;
}

int main() {
  Textile textile = {0};
  register_pattern(&textile, "TEST PATTERN");
  begin_textile(process, &textile);
  return 0;
}
