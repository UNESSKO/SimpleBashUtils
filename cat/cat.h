#ifndef CAT_H
#define CAT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char ch;
  int flag_enable;
  int b_flag;
  int e_flag;
  int n_flag;
  int s_flag;
  int t_flag;
  int v_flag;
  int start_index;
} CatOption;

void parser(int argc, char *argv[], CatOption *options);
FILE *openFile(char *path, char *type);
void readFile(FILE *file);
void copyFile(FILE *main_file, FILE *temp_file);
void bFlag(FILE *file);
void vFlag(FILE *file);
void eFlag(FILE *file);
void nFlag(FILE *file);
void s_Flag(FILE *file);
void tFlag(FILE *file, int b_flag, int n_flag);

#endif