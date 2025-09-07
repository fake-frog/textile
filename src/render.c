#include "include.h"
#include <stdio.h>

void render_pattern(PatternMap *map, char *name) {
  Pattern *pattern = get_pattern(map, name);

  printf("%s \r\n", pattern->sequene);
}
