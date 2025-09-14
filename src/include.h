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

typedef enum { OVERFLOW, BREAK_WORD, BREAK_CHAR } Warp;
typedef enum { LEFT, RIGHT, UP, DOWN } Direction;

typedef struct {
  int x;
  int y;
  Stich stich;
} Needle;

// add wrapping modes
typedef struct {
  char name[MAX_MAP_KEY_SIZE];
  int x;
  int y;
  Needle needle;
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
  int active_pattern;
} Textile;

void register_pattern(Textile *textile, const char *name);
Pattern *get_pattern(Textile *textile, char *name);
void sow(char *string, Pattern *pattern);

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
void sow_point(Needle *needle, char *string);
void sow_char(Needle *needle, char *string);
void sow_word(Needle *needle, char *string);
void move_needle(Needle *needle, int x, int y);
void return_needle(Needle *needle);
void reset_needle(Needle *needle);
// term utils

typedef struct {
  unsigned int char_x;
  unsigned int char_y;
  unsigned int pixel_x; // not always reported by terminal
  unsigned int pixel_y; // not always reported by terminal
} window_size;

void move_cursor(int x, int y);
void clear_screen();
void disable_raw_mode();
void enable_raw_mode();
void switch_to_back_buffer();
void begin_textile(int (*process)(double, Textile *), Textile *textile);
void switch_to_main_buffer();
window_size get_window_size();

#endif
