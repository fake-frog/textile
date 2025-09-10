#include "include.h"

int get_index(PatternMap map, char key[]) {
  for (int i = 0; i < map.length; i++) {
    if (strlen(key) <= MAX_MAP_KEY_LENGTH && strcmp(map.keys[i], key) == 0) {
      return i;
    }
  }
  return -1;
}

int insert_pattern(PatternMap *map, char key[], Pattern pattern) {
  int index = get_index(*map, key);
  if (index < 0) {
    if (map->length >= MAX_PATTERN_LENGTH) {
      return 1;
    }
    index = map->length++;
  }
  map->patterns[index] = pattern;
  strcpy(map->keys[index], key);
  return 0;
}

Pattern *get_pattern(PatternMap *map, char *key) {
  int index = get_index(*map, key);
  return (index >= 0) ? &map->patterns[index] : NULL;
}
