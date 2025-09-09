#include "include.h"

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
**
** textile will handle 3 things
**
** 1. rendering text frame by frame
** 2. window focus and interaction
** 3. tiling and window overflow
**
** =========================================================================
*/

// Start renderloop
void begin_textile(int (*process)(double)) {
  enable_raw_mode();
  clear_screen();
  fflush(stdout);

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

    int ERROR = process(delta_time / (double)BILLION);
    fflush(stdout); // see the output
    if (ERROR) {
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

// The needle will exicute the given stich command
// on the location on hte patterns sequnce
void sow(char *string, Pattern *pattern) {
  switch (pattern->needle.stich) {
  case POINT:
    sow_point(&pattern->needle, string);
    break;
  case CHAR:
    sow_char(&pattern->needle, string);
    break;
  case WORD:
    sow_word(&pattern->needle, string);
    break;
  case LINE:
    sow_line(&pattern->needle, string);
    break;
  case MARK:
    break;
  }
}

void register_patten(Textile *textile, Pattern *pattern) {
  insert_pattern(&textile->patternMap, pattern->name, *pattern);
}

// putes patterns together
void stitch_to(Textile textile, char *pattern1_name, char *pattern2_name,
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
