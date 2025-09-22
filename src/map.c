#include "include.h"

int get_index(PatternMap map, const char *key) {
  for (int i = 0; i < map.length; i++) {
    if (strlen(key) <= MAX_MAP_KEY_LENGTH && strcmp(map.keys[i], key) == 0) {
      return i;
    }
  }
  return -1;
}

int insert_value(PatternMap *map, const char *key, Pattern pattern) {
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

int remove_value(PatternMap *map, const char *key) {
  int index = get_index(*map, key);
  if (index < 0)
    return 1; /* not found */
  for (int i = index; i < map->length - 1; i++) {
    map->patterns[i] = map->patterns[i + 1];
    strcpy(map->keys[i], map->keys[i + 1]);
  }
  map->length--;
  map->keys[map->length][0] = '\0';
  return 0;
}

Pattern *get_value(PatternMap *map, const char *key) {
  int index = get_index(*map, key);
  return (index >= 0) ? &map->patterns[index] : NULL;
}
