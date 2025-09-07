#include "include.h"

int process(double delta_time) {
  clear_screen();

  //  printf("DELTA TIME: %f\r\n", delta_time);

  Textile textile = {0};

  Pattern pattern = {
      .needle = {1, 1, LINE}, // the terminal starts at 1,1 for some reason
      .name = "TEST PATTERN",
      .sequene = "TEST SEQUENE FOR TEST PATTERN",
  };

  register_patten(&textile, &pattern);


  pattern.needle.x = 1;
  pattern.needle.y = 1;

  pattern.needle.stich = CHAR;
  sow("C", &pattern);

  pattern.needle.stich = WORD;
  sow("WORD", &pattern);
  sow("WORD", &pattern);
  sow("WORD", &pattern);
  sow("WORD", &pattern);

  pattern.needle.stich = LINE;
  sow("THIS IS A LINE", &pattern);

  pattern.needle.stich = WORD;
  sow("WORD", &pattern);

  return 0;
}

int main() {
  begin_textile(process);
  return 0;
}
