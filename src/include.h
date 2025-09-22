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
  int buff_pos;
} Needle;

typedef struct {
  char *buff;
  int buff_size;
  int buff_length;
  int line_count;
  Needle needle;
} Sequence;

int get_sequence_buff_line_count(char *buff, int buff_len);
Sequence *init_sequnce(int buff_size);
void remove_sequence(Sequence *s);
void set_sequence_buffer(Sequence *s, const char *buff, int buff_size);

// add wrapping modes
typedef struct {
  char name[MAX_MAP_KEY_SIZE];
  Sequence *sequence;
  Needle needle;
  int order; // set by user - sets drawing order
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
Pattern *get_value(PatternMap *map, const char *key);
int remove_value(PatternMap *map, const char *key);

typedef struct {
  PatternMap pattern_map;
} Textile;

void weave_patterns(Textile *textile);
void register_pattern(Textile *textile, const char *name);
void unregister_pattern(Textile *textile, const char *name);
void free_textile(Textile *texile);
Pattern *get_pattern(Textile *textile, const char *name);
void set_pattern_active(Textile *textile, const char *name);
void set_pattern_inactive(Textile *textile, const char *name);
void sow(Textile *textile, char *stich, const char *pattern_name);

/*
** ===============================
**
** INERNAL - not to be exposed
**
** ===============================
*/

// sequence
void reset_sequence(Pattern *pattern);
int get_sequence_buff_line_count(char *buff, int buff_len);
Sequence *init_sequnce(int buff_size);
void remove_sequence(Sequence *s);
void set_sequence_buffer(Sequence *s, const char *buff, int buff_size);

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
