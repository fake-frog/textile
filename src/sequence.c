#include "include.h"

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

Sequence *init_sequnce(int buff_size) {
  Sequence *s = malloc(sizeof(Sequence));
  if (!s)
    return NULL;
  char *b = calloc(buff_size, 1);
  if (!b) {
    free(s);
    return NULL;
  }
  s->buff = b;
  s->buff_size = buff_size;
  s->needle.x = 0;
  s->needle.y = 0;
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
