#ifndef GREP_H
#define GREP_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATTERNS 1024
#define MAX_LINE_LENGTH 1024

typedef struct {
  char *patterns[MAX_PATTERNS];
  int pattern_count;
  int show_line_numbers;
  int only_matching;
  int c_flag;
  int list_files;
  int start_index;
  int hide_filenames;
  int ignore_case;
  int inverted;
  int suppress_errors;
  int trigger;
  int f_flag;
} GrepOptions;

void parse_arguments(int argc, char *argv[], GrepOptions *options);
void matching(char *line, regex_t *regex);
char *getLastSym(char *line);
void print_flag(int *lTriggered, int line_number, char *line, char *file,
                regex_t *regex, int file_count, GrepOptions *options);
void process(FILE *input, char *file, regex_t *regex, int file_count,
             GrepOptions *options);
void read_patterns_from_file(char *filename, char *patterns[],
                             int *pattern_count);

#endif