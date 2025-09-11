#include "include.h"
#include <string.h>

#define FPS 60

/* NOTE - ABOUT TEXTILE
** =========================================================================
** Textile is a minimal library for making terminal application. Define pat-
** terns and tell textile how they should be stiched together. Textile works
** in a similar way to tilling window mangers.
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
  fflush(stdout);
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
        // printf("Quitting...\r\n");
        //  TODO - handle exit
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

void reset_sequence(Pattern *pattern) { pattern->sequence[0] = '\n'; }

static void capture_string(char *string, char *leading_char,
                           char *trailing_char, Pattern *pattern) {
  if (strlen(pattern->sequence) + strlen(string) >= MAX_SEQUENCE_SIZE)
    return;

  char old_sequence[MAX_SEQUENCE_SIZE];
  strcpy(old_sequence, pattern->sequence);
  snprintf(pattern->sequence, sizeof(pattern->sequence), "%s%s%s%s",
           leading_char, old_sequence, string, trailing_char);
}

// The needle will exicute the given stich command
// on the location on hte patterns sequnce
void sow(char *string, Pattern *pattern) {
  switch (pattern->needle.stich) {
  case POINT:
    sow_point(&pattern->needle, string);
    // TODO need some sort of replace at string function
    break;
  case CHAR:
    sow_char(&pattern->needle, string);
    capture_string(string, "", "", pattern);
    break;
  case WORD:
    sow_word(&pattern->needle, string);
    capture_string(string, "", " ", pattern);
    break;
  case LINE:
    sow_line(&pattern->needle, string);
    char *leading_char = pattern->needle.x > 1 ? "\n" : "";
    capture_string(string, leading_char, "\n", pattern);
    break;
  case MARK:
    // This is going to be a weird one to capture
    // have to find the squence char pos of the mark
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
