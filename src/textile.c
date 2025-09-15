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
#define SOW_WITH_CURSOR(sow_func, needle, string, offset_x, offset_y)          \
  (move_cursor((needle)->x + offset_x, (needle)->y + (offset_y)),              \
   sow_func(needle, string))

// Start renderloop
void begin_textile(int (*process)(double, Textile *), Textile *textile) {
  enable_raw_mode();
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

    update_pattern_size(textile);
    int ERROR = process(delta_time / (double)BILLION, textile);
    if (ERROR) {
      printf("ERROR IN: process\r\n");
    }
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

void sow(Textile *textile, char *stich, char *pattern_name) {
  Pattern *pattern = get_pattern(textile, pattern_name);

  if (!is_pattern_active(textile, pattern_name))
    return;
  if (pattern->needle.x > pattern->width)
    return;

  switch (pattern->needle.stich) {
  case POINT:
    SOW_WITH_CURSOR(sow_point, &pattern->needle, stich, pattern->x, pattern->y);
    break;
  case CHAR:
    SOW_WITH_CURSOR(sow_char, &pattern->needle, stich, pattern->x, pattern->y);
    break;
  case WORD:
    SOW_WITH_CURSOR(sow_word, &pattern->needle, stich, pattern->x, pattern->y);
    break;
  }
}

int is_pattern_active(Textile *textile, char *pattern_name) {
  for (int i = 0; i < textile->active_pattern_length; i++) {
    if (strcmp(textile->active_patterns[i], pattern_name) == 0) {
      return 1;
    }
  }
  return 0;
}

void update_pattern_pos(Textile *textile) {
  for (int i = 0; i < textile->active_pattern_length; i++) {
    Pattern *active_pattern = get_pattern(textile, textile->active_patterns[i]);
    active_pattern->x = active_pattern->order * active_pattern->width;
    active_pattern->y = 0;
  }
}

void update_pattern_size(Textile *textile) {
  if (textile->active_pattern_length < 1)
    return;
  WindowSize ws = get_window_size();
  int pattern_width = ws.char_x / textile->active_pattern_length;
  // int pattern_hight = ws.char_y / textile->active_pattern_length;
  int pattern_hight = ws.char_y;

  for (int i = 0; i < textile->active_pattern_length; i++) {
    Pattern *active_pattern = get_pattern(textile, textile->active_patterns[i]);
    active_pattern->width = pattern_width;
    active_pattern->height = pattern_hight;
  }
}

void register_pattern(Textile *textile, const char *name) {
  WindowSize ws = get_window_size();
  Pattern pattern = {
      .needle = {1, 1, WORD}, // the terminal starts at 1,1 for some reason
      .order = -1,
      .x = 0,
      .y = 0,
      .width = ws.char_x,
      .height = ws.char_y};
  strcpy(pattern.name, name);
  insert_value(&textile->pattern_map, pattern.name, pattern);
}

Pattern *get_pattern(Textile *textile, char *name) {
  Pattern *pattern = get_value(&textile->pattern_map, name);
  return pattern ? pattern : NULL;
}

/* patterns together
void weave_to(Textile textile, char *pattern1_name, char *pattern2_name,
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
*/
