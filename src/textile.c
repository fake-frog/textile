#include "include.h"
#include <bits/time.h>
#include <fcntl.h>
#include <time.h>
#define FPS 60

void begin_textile(int (*process)()) {
  enable_raw_mode();
  clear_screen();

  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

  long nano_target_frame_time = (long)(1.0 / FPS * 1000000000L); // nano seconds
  struct timespec tstart, tend, tsleep;
  tsleep.tv_sec = 0; // we dont need this

  long delta_time = 0;

  char c;
  while (1) {

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    int bytes_read = read(STDIN_FILENO, &c, 1);

    if (bytes_read == 1) {
      if (c == 'q') {
        printf("Quitting...\r\n");
        break;
      }
    }

    if (process((double)(delta_time / 1000000000.0)) == 1) {
      break;
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);

    delta_time = (tend.tv_sec - tstart.tv_sec) * 1000000000L +
                 (tend.tv_nsec - tstart.tv_nsec);

    tsleep.tv_nsec = 0;

    if (delta_time < nano_target_frame_time) {
      tsleep.tv_nsec = nano_target_frame_time - delta_time;
    }

    nanosleep(&tsleep, NULL);
  }
}

void append_block(Pattern *pattern, Block block) {
  // TODO check if curr_block_index is larger than max size
  pattern->weave[pattern->curr_weave_index] = block;
  pattern->curr_weave_index++;
}

// weft is horizontal
void sequence_weft(Pattern *pattern, char sequece[MAX_SEQUENCE_SIZE]) {
  Block block = {.shuttle_pos = pattern->curr_position_shuttle,
                 .sequence_type = 0,
                 .sequece = sequece};
  append_block(pattern, block);
}

typedef enum { LEFT, RIGHT, UP, DOWN } Direction;

void stitch(Pattern *active_pattern, Pattern *new_pattern,
            Direction direction) {

  switch (direction) {
  case LEFT:
    // Calc size for the new pattern slit and add to the left
    break;
  case RIGHT:
    // add to right
    break;
  case UP:
    // add to top
    break;
  case DOWN:
    // add to bottom
    break;
  }
}

/*
TODO

Heres the idea

struct

Textile textile = {0};
Pattern pattern = {0};

sequence_weft(pattern, "*-->");
sequence_warp(pattern, "-->");
sequence_weft(pattern, "[b]"); // this is a button maybe?

weft - horizontal
warp - vertical

equals

*-->
|[b]
|
∨

pattern_register("pattern name", pattern, textile);


*/
