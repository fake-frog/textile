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
typedef enum {
  POINT,
  CHAR,
  WORD,
} Stich;

typedef enum { HIDE, BREAK_WORD, BREAK_CHAR } Overflow;
typedef enum { LEFT, RIGHT, UP, DOWN } Direction;

typedef struct {
  int x;
  int y;
  Stich stich;
} Needle;

// add wrapping modes
typedef struct {
  char name[MAX_MAP_KEY_SIZE];
  Needle needle;
  int order;
  int x;
  int y;
  int width;
  int height;
} Pattern;

typedef struct {
  char keys[MAX_MAP_KEY_LENGTH][MAX_MAP_KEY_SIZE];
  Pattern patterns[MAX_PATTERN_LENGTH];
  int length;
} PatternMap;

int get_index(PatternMap map, const char *key);
// returns 1 if you overflow
int insert_value(PatternMap *map, const char *key, Pattern);
Pattern *get_value(PatternMap *map, char *key);

typedef struct {
  PatternMap pattern_map;
  char active_patterns[MAX_MAP_KEY_LENGTH][MAX_SEQUENCE_SIZE];
  int active_pattern_length;
} Textile;

void register_pattern(Textile *textile, const char *name);
void update_pattern_size(Textile *textile);
Pattern *get_pattern(Textile *textile, char *name);
int is_pattern_active(Textile *textile, char *pattern_name);
void sow(Textile *textile, char *stich, char *pattern_name);

/*
** ===============================
**
** INERNAL - not to be exposed
**
** ===============================
*/

// sequence
void reset_sequence(Pattern *pattern);

// needle actions
void sow_point(Needle *needle, char *stich);
void sow_char(Needle *needle, char *stich);
void sow_word(Needle *needle, char *stich);
void move_needle(Needle *needle, int x, int y);
void return_needle(Needle *needle);
void reset_needle(Needle *needle);
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
void switch_to_back_buffer();
void begin_textile(int (*process)(double, Textile *), Textile *textile);
void switch_to_main_buffer();
WindowSize get_window_size();

#endif
