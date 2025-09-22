#include "include.h"

int process(double delta_time, Textile *textile) {
  clear_screen();
  sow(textile, "test", "test");
  return 0;
}

int main() {
  Textile textile = {0};
  register_pattern(&textile, "test");
  begin_textile(process, &textile);
  free_textile(&textile);
  return 0;
}
