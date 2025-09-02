#include "include.h"

int process(double delta_time) {
  clear_screen();

  printf("DELTA TIME: %f\n", delta_time);
  return 0;
}

int main() {
  begin_textile(process);
  return 0;
}
