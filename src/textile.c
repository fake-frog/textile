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

#include "include.h"
#define FPS 60

// Start renderloop
void begin_textile(int (*process)(double, Textile *), Textile *textile) {
  enable_raw_mode(); // side effect: switches to back buffer
  fflush(stdout);
  clear_screen();

  // make the input nonblocking
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
    int ERROR = process(delta_time / (double)BILLION, textile);
    if (ERROR) {
      printf("ERROR IN: process\r\n");
    }
    weave_patterns(textile);
    fflush(stdout); // see the output

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

void sow(Textile *textile, char *stich, const char *pattern_name) {
  Pattern *p = get_pattern(textile, pattern_name);
  const char test_s[100] = "Test sequence";
  set_sequence_buffer(p->sequence, test_s, 100);
}

// each pattern gets its own printf
void weave_patterns(Textile *textile) {
  for (int i = 0; i < textile->pattern_map.length; i++) {
    Pattern *p = get_pattern(textile, textile->pattern_map.keys[i]);
    if (!p)
      continue;
    printf("%s", p->sequence->buff);
  }
}

void register_pattern(Textile *textile, const char *name) {
  WindowSize ws = get_window_size();
  Sequence *sequence = init_sequnce(MAX_SEQUENCE_SIZE);
  Pattern pattern = {
      .sequence = sequence,
      .needle = {1, 1, WORD}, // the terminal starts at 1,1 for some reason
      .order = 0,
      .x = 0,
      .y = 0,
      .width = ws.char_x,
      .height = ws.char_y};
  strcpy(pattern.name, name);
  insert_value(&textile->pattern_map, pattern.name, pattern);
}

void unregister_pattern(Textile *textile, const char *name) {
  Pattern *p = get_pattern(textile, name);
  remove_sequence(p->sequence);
  remove_value(&textile->pattern_map, name);
}

Pattern *get_pattern(Textile *textile, const char *name) {
  Pattern *p = get_value(&textile->pattern_map, name);
  return p ? p : NULL;
}

void free_textile(Textile *textile) {
  for (int i = 0; i < textile->pattern_map.length; i++) {
    Pattern *p = get_pattern(textile, textile->pattern_map.keys[i]);
    if (!p)
      continue;
    remove_sequence(p->sequence);
  }
}
