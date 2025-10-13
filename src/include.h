#ifndef INCLUDE_H
#define INCLUDE_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define MAX_SEQUENCE_SIZE 1024
#define MAX_PATTERN_LENGTH 1024
#define MAX_MAP_KEY_LENGTH 1024
#define MAX_MAP_KEY_SIZE 100

#define MAX_LINE_LENGTH 2048
/*
** ===============================
**
** API ITEMS - for users
**
** ===============================
*/

// stitch were place characters
// its where we begin writing and
// how we proceed.
//

typedef struct {
  char *buff;
  int buff_size;
  int buff_length;
  int line_count;
  int line_length[MAX_LINE_LENGTH];
} Sequence;

int get_sequence_buff_line_count(char *buff, int buff_len);
void remove_sequence(Sequence *s);
void set_sequence_buffer(Sequence *s, const char *buff, int buff_size);
void append_to_sequence_buffer(Sequence *s, const char *string);

typedef struct {
  char name[MAX_MAP_KEY_SIZE];
  Sequence *sequence;
  int width;
  int height;
} Pattern;

typedef struct {
  char keys[MAX_MAP_KEY_LENGTH][MAX_MAP_KEY_SIZE];
  Pattern patterns[MAX_PATTERN_LENGTH];
  int length;
} PatternMap;

// pattern map
int get_index(PatternMap map, const char *key);
// returns 1 if you overflow
int insert_value(PatternMap *map, const char *key, Pattern);
Pattern *get_value(PatternMap *map, const char *key);
int remove_value(PatternMap *map, const char *key);

typedef struct {
  PatternMap pattern_map;
  char *surface;
  int width;
  int height;
} Textile;

Textile *init_textile();

void set_surface_at(Textile *textile, int x, int y);
void set_surface_between(Textile *textile, int x1, int y1, int x2, int y2);
void print_surface(Textile *textile);
void clear_surface(Textile *textile, int width, int height);
void weave_at(Textile *textile, Pattern *pattern, int x, int y);
void register_pattern(Textile *textile, const char *name);
void unregister_pattern(Textile *textile, const char *name);
void free_textile(Textile *texile);
Pattern *get_pattern(Textile *textile, const char *name);
void set_pattern_active(Textile *textile, const char *name);
void sow(Textile *textile, const char *pattern_name,
         void (*stich)(Pattern *, const char *), char *string);

/*
** ===============================
**
** INERNAL - not to be exposed
**
** ===============================
*/

// string utils
char *blank_buffer(int width, int height);

// sequence
void reset_sequence(Pattern *pattern);
int get_sequence_buff_line_count(char *buff, int buff_len);
char *truncate_lines(char *buff, int width, int height);
Sequence *init_sequnce(int width, int height);
void remove_sequence(Sequence *s);
void set_sequence_buffer(Sequence *s, const char *buff, int buff_size);

// term utils

typedef struct {
  unsigned int char_x;
  unsigned int char_y;
  unsigned int pixel_x; // not always reported by terminal
  unsigned int pixel_y; // not always reported by terminal
} WindowSize;

void move_cursor(int x, int y);
void clear_screen();
void disable_raw_mode();
void enable_raw_mode();
void non_blocking_input();
void switch_to_back_buffer();
void begin_textile(int (*process)(double, Textile *), Textile *textile,
                   int fps);
void switch_to_main_buffer();
WindowSize get_window_size();

#endif
