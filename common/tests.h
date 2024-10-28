#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 1024

FILE *actual_output(char *path);
char *read_actual_output(FILE *fp);
void result(char *test_name, int result);
void test(char *test_name, char *grep, char *path);

#endif  // TESTS_H
