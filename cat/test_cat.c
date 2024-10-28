#include "../common/tests.c"

int main() {
  FILE *file = fopen("test.txt", "w");
  fprintf(file, "123\n123\n\n\n1\t2\t3\t");
  fclose(file);

  test("Test print", "cat test.txt", "./s21_cat test.txt");
  test("Test b flag", "cat -b test.txt", "./s21_cat -b test.txt");
  test("Test s flag", "cat -s test.txt", "./s21_cat -s test.txt");
  test("Test e flag", "cat -e test.txt", "./s21_cat -E test.txt");
  test("Test n flag", "cat -n test.txt", "./s21_cat -n test.txt");
  test("Test t flag", "cat -t test.txt", "./s21_cat -t test.txt");
  test("Test s t flag", "cat -s -t test.txt", "./s21_cat -s -t test.txt");
  test("Test n t flag", "cat -n -t  test.txt", "./s21_cat -n -t test.txt");
  test("Test n b  flag", "cat -n -b  test.txt", "./s21_cat -n -b  test.txt");
  test("Test e b s flag", "cat -e -b -s test.txt",
       "./s21_cat -e -b -s  test.txt");
  test("Test n t b flag", "cat -n -t -b  test.txt",
       "./s21_cat -n -t -b  test.txt");
  test("Test s e n t flag", "cat -s -e -n -t  test.txt",
       "./s21_cat -s -e -n -t  test.txt");
  test("Test t b s e n  flag", "cat -t -b -s -e -n  test.txt",
       "./s21_cat -t -b -s -e -n  test.txt");

  remove("test.txt");

  return 0;
}