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

// Start renderloop
void begin_textile(int (*process)(double, Textile *), Textile *textile,
                   int fps) {

  enable_raw_mode();
  fflush(stdout);
  clear_screen();

  non_blocking_input();
  long BILLION = 1000000000L;

  long nano_target_frame_time = (long)(1.0 / fps * BILLION); // nano seconds
  struct timespec tstart, tend, tsleep;
  tsleep.tv_sec = 0;
  long delta_time = 0;

  char c;
  while (1) {
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    int bytes_read = read(STDIN_FILENO, &c, 1);

    if (bytes_read == 1) {
      if (c == 'q') {
        break;
      }
    }

    int ERROR = process(delta_time / (double)BILLION, textile);
    if (ERROR) {
      printf("ERROR IN: process\n\r");
    }

    print_surface(textile);
    fflush(stdout);

    clock_gettime(CLOCK_MONOTONIC, &tend);

    tsleep.tv_nsec = 0;
    delta_time = (tend.tv_sec - tstart.tv_sec) * BILLION +
                 (tend.tv_nsec - tstart.tv_nsec);

    if (delta_time < nano_target_frame_time) {
      tsleep.tv_nsec = nano_target_frame_time - delta_time;
      delta_time = nano_target_frame_time;
    }

    nanosleep(&tsleep, NULL);
  }
}

void sow(Textile *textile, const char *pattern_name,
         void (*stich)(Pattern *, const char *), char *string) {

  Pattern *pattern = get_pattern(textile, pattern_name);
  if (!pattern)
    return;

  stich(pattern, string);
  weave_at(textile, pattern, 1, 1);
}

void weave_at(Textile *textile, Pattern *pattern, int x, int y) {
  sprintf(textile->surface, "%s", pattern->sequence->buff);
}

void print_surface(Textile *textile) { printf("%s", textile->surface); }

void clear_surface(Textile *textile, int width, int height) {
  textile->surface = blank_buffer(width, height);
}

void register_pattern(Textile *textile, const char *name) {
  WindowSize ws = get_window_size();
  Sequence *sequence = init_sequnce(ws.char_x, ws.char_y);
  Pattern pattern = {
      .sequence = sequence, .width = ws.char_x, .height = ws.char_y};
  strcpy(pattern.name, name);
  insert_value(&textile->pattern_map, pattern.name, pattern);
}

void unregister_pattern(Textile *textile, const char *name) {
  Pattern *p = get_pattern(textile, name);
  if (!p)
    return;
  remove_sequence(p->sequence);
  remove_value(&textile->pattern_map, name);
}

Pattern *get_pattern(Textile *textile, const char *name) {
  Pattern *p = get_value(&textile->pattern_map, name);
  return p ? p : NULL;
}

void set_surface_at(Textile *textile, int x, int y) {}
void set_surface_between(Textile *textile, int x1, int y1, int x2, int y2) {}

Textile *init_textile() {
  Textile *t = malloc(sizeof(Textile));
  if (!t)
    return NULL;
  WindowSize ws = get_window_size();
  char *s = blank_buffer(ws.char_x, ws.char_y);
  return t;
}

void free_textile(Textile *textile) {
  for (int i = 0; i < textile->pattern_map.length; i++) {
    Pattern *p = get_pattern(textile, textile->pattern_map.keys[i]);
    if (!p)
      continue;
    remove_sequence(p->sequence);
  }
  free(textile->surface);
}
