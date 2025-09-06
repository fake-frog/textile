#include "include.h"
#include <stdio.h>

void render_pattern(PatternMap *map, char *name) {
  Pattern *pattern = get_pattern(map, name);

  printf("%d \r\n", map->length);
  printf("%s \r\n", pattern->name);
}
