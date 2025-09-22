#include "include.h"

void cut_sequence(char *sequence, int end) {}

void go_to_buff_pos(Needle *needle, char *sequence_buff, int sequence_len) {

  int line_num = needle->y;
  int i = 0;
  while (line_num) {
    if (i >= sequence_len)
      break;

    char c = sequence_buff[i];

    if (c == '\n') {
      line_num--;
    }
    needle->buff_pos++;
  }

  // if the x pos is > the line lenth, just go to the end of the line and return
  for (int j = 0; j < needle->x && j < sequence_len; j++) {
    char c = sequence_buff[i];
    if (c == '\n')
      return;

    needle->buff_pos++;
  }
}
