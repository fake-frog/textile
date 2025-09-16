#include "include.h"

int process(double delta_time, Textile *textile) {
  clear_screen();

  //  printf("DELTA TIME: %f\r\n", delta_time);

  WindowSize ws = get_window_size();

  char message[128];
  char pattern_name[40] = "TEST PATTERN";
  Pattern *pattern = get_pattern(textile, pattern_name); // can return null

  if (!pattern) {
    return 1;
  }
  reset_needle(&pattern->needle);

  sprintf(message, "char x -> %d", ws.char_x);
  sow(textile, message, pattern_name);
  return_needle(&pattern->needle);

  sprintf(message, "char y -> %d", ws.char_y);
  sow(textile, message, pattern_name);
  return_needle(&pattern->needle);

  sprintf(message, "pixel x -> %d", ws.pixel_x);
  sow(textile, message, pattern_name);
  return_needle(&pattern->needle);

  sprintf(message, "pixel y -> %d", ws.pixel_y);
  sow(textile, message, pattern_name);

  return 0;
}

int main() {
  Textile textile = {0};
  register_pattern(&textile, "TEST PATTERN");
  register_pattern(&textile, "TEST PATTERN 2");
  unregister_pattern(&textile, "TEST PATTERN 2");
  set_pattern_active(&textile, "TEST PATTERN");
  set_pattern_active(&textile, "TEST PATTERN 2");
  set_pattern_inactive(&textile, "TEST PATTERN");
  set_pattern_active(&textile, "TEST PATTERN");
  begin_textile(process, &textile);
  return 0;
}
