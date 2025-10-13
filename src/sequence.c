#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_sequence_buff_line_count(char *buff, int buff_len) {
  int line_count = 0;
  for (int i = 0; i < buff_len; i++) {
    char c = buff[i];
    if (c == '\n') {
      line_count++;
    }
    i++;
  }
  return line_count;
}

Sequence *init_sequnce(int width, int height) {
  Sequence *s = malloc(sizeof(Sequence));
  if (!s)
    return NULL;
  char *b = blank_buffer(width, height);
  if (!b) {
    free(s);
    return NULL;
  }
  s->buff = b;
  s->buff_size = width * height;
  return s;
}

void remove_sequence(Sequence *s) {
  if (!s)
    return;
  free(s->buff);
  free(s);
}

void set_sequence_buffer(Sequence *s, const char *buff, int buff_size) {
  if (buff_size >= s->buff_size) {
    int new_size = (int)(1.5 * buff_size);
    s->buff = calloc(new_size, 1);
    s->buff_size = new_size;
  }
  strcpy(s->buff, buff);
  s->buff_length = strlen(s->buff);
  s->line_count = get_sequence_buff_line_count(s->buff, s->buff_length);
}

void append_to_sequence_buffer(Sequence *s, const char *string) {
  int string_length = strlen(string);
  if (s->buff_length + string_length > s->buff_size) {
    return;
  }
  char *p = s->buff + s->buff_length;
  p += sprintf(p, "%s", string);
  s->buff_length += string_length;
}

// this is half baked
char *truncate_lines(char *buff, int width, int height) {
  char *return_buff = blank_buffer(width, height);
  int return_buff_len = strlen(return_buff);
  int buff_len = strlen(return_buff);
  int line_len = 0;
  int i = 0;
  // copy
  while (i < return_buff_len || i < buff_len) {
    line_len++;
    char c = buff[i];
    return_buff[i] = c;
    if (c == '\n') {
      line_len = 0;
      return_buff[++i] = '\r';
    }
    if (line_len == width) {
      return_buff[i] = '$';
      line_len = 0;
      // skip to next line
      while (c != '\n' && i < buff_len - 1) {
        c = buff[++i];
      }
    }
    i++;
  }
  return return_buff;
}
