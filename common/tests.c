#include "tests.h"

FILE *actual_output(char *path) {
  FILE *fp = popen(path, "r");

  if (fp == NULL) {
    perror("Ошибка при запуске программы");
    exit(1);
  }

  return fp;
}

char *read_actual_output(FILE *fp) {
  char *content = (char *)malloc(LEN);
  size_t bytes_read = fread(content, sizeof(char), LEN, fp);
  if (bytes_read < LEN) content[bytes_read] = '\0';

  return content;
}

void result(char *test_name, int result) {
  printf("%s: %s\n", test_name, result ? "SUCCESS" : "FAILED");
}

void test(char *test_name, char *grep, char *path) {
  FILE *file = actual_output(path);
  char *actual_out = read_actual_output(file);

  FILE *gp = actual_output(grep);
  char *grep_out = read_actual_output(gp);

  if (strcmp(grep_out, actual_out) == 0)
    result(test_name, 1);
  else
    result(test_name, 0);

  free(actual_out);
  free(grep_out);
}