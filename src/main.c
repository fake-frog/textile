#include "include.h"
#include <stdlib.h>
#include <unistd.h>

void stich_end(Pattern *pattern, const char *string) {
  append_to_sequence_buffer(pattern->sequence, string);
}

void stich_begining(Pattern *pattern, const char *string) {
  pattern->sequence->buff[0] = string[0];
}

int process(double delta_time, Textile *textile) {
  clear_screen();
  WindowSize w = get_window_size();
  clear_surface(textile, w.char_x, w.char_y);
  // sow(textile, "test", stich_end, "*");
  return 0;
}

int main() {
  WindowSize w = get_window_size();
  char *surface = blank_buffer(w.char_x, w.char_y);
  Textile textile = {0};
  textile.surface = surface;
  register_pattern(&textile, "test");
  begin_textile(process, &textile, 10);
  free_textile(&textile);
  return 0;
}
