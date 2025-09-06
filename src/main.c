#include "include.h"

int process(double delta_time) {
  clear_screen();

  printf("DELTA TIME: %f\r\n", delta_time);

  Textile textile = {0};
  Pattern pattern = {
      .name = "TEST PATTERN",
      .sequene = "TEST SEQUENE FOR TEST PATTERN",
  };

  register_patten(&textile, &pattern);
  render_pattern(&textile.patternMap, "TEST PATTERN");

  return 0;
}

int main() {
  begin_textile(process);
  return 0;
}
