#include "include.h"

char *blank_buffer(int width, int height) {
  const int buffer_size = width * height;
  char *buffer = calloc(buffer_size, 1);
  memset(buffer, '*', width * height);
  buffer[width * height] = '\0';
  return buffer;
}
