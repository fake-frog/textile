#include "include.h"
#include <bits/time.h>
#include <fcntl.h>
#include <time.h>
#define FPS 60

/* NOTE - ABOUT TEXTILE
** =========================================================================
** Textile is a minimal library for making terminal application. Define pat-
** terns and tell textile how they should be stiched together. Textile works
** in a simular way to tilling window manger.
**
** stitch_left(...)
** ----
** will stitch a pattern to the left of the most recelty defined pattern.
**
** stitch_left_all(...)
** ----
** will stich a pattern to the left of all of the current pattern.
**
** and so on...
** =========================================================================
*/

// Start renderloop
void begin_textile(int (*process)(double)) {
  enable_raw_mode();
  clear_screen();

  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

  long BILLION = 1000000000L;

  long nano_target_frame_time = (long)(1.0 / FPS * BILLION); // nano seconds
  struct timespec tstart, tend, tsleep;
  tsleep.tv_sec = 0; // we dont use this
  long delta_time = 0;

  char c;
  while (1) {
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    int bytes_read = read(STDIN_FILENO, &c, 1);

    if (bytes_read == 1) {
      if (c == 'q') {
        printf("Quitting...\r\n");
        // TODO - handle exit
        break;
      }
    }

    if (process(delta_time / (double)BILLION) == 1) {
      // TODO - handle error
      break;
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);

    tsleep.tv_nsec = 0;
    delta_time = (tend.tv_sec - tstart.tv_sec) * BILLION +
                 (tend.tv_nsec - tstart.tv_nsec);

    if (delta_time < nano_target_frame_time) {
      tsleep.tv_nsec = nano_target_frame_time - delta_time;
      delta_time = nano_target_frame_time; // trust the sleep time
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
