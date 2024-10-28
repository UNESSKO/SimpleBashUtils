#include "../common/tests.c"

int main() {
  FILE *file = fopen("test.txt", "w");
  FILE *file2 = fopen("test2.txt", "w");
  fprintf(file, "123\nHeLlo WorlD\n\n-321");
  fprintf(file2, "123\nHeLlo WorlD\n\n-321");
  fclose(file);
  fclose(file2);

  test("Test non flag", "grep 12 test.txt", "./s21_grep 12 test.txt");
  test("Test e flag", "grep -e 1 -e -3 test.txt",
       "./s21_grep -e 1 -e -3 test.txt");  // -e -3
  test("Test i flag", "grep -i hello test.txt", "./s21_grep -i hello test.txt");
  test("Test v flag", "grep -v 3 test.txt", "./s21_grep -v 3 test.txt");
  test("Test c flag", "grep -c 321 test.txt", "./s21_grep -c 321 test.txt");
  test("Test l flag", "grep -l 1 test.txt", "./s21_grep -l 1 test.txt");
  test("Test n flag", "grep -n H test.txt", "./s21_grep -n H test.txt");
  test("Test h flag", "grep -h 123 test.txt test2.txt",
       "./s21_grep -h 123 test.txt test2.txt");
  remove("test2.txt");
  test("Test s flag", "grep -s 3 test", "./s21_grep -s 3 test");

  FILE *test_f = fopen("test_reg.txt", "w");
  fprintf(test_f, "[a-z]\n123");
  fclose(test_f);
  test("Test f flag", "grep -f test_reg.txt test.txt",
       "./s21_grep -f test_reg.txt test.txt");
  remove("test_reg.txt");
  test("Test o flag", "grep -o 2 test.txt", "./s21_grep -o 2 test.txt");
  test("Test iv flag", "grep -iv 1 test.txt", "./s21_grep -iv 1 test.txt");
  test("Test in flag", "grep -in 123 test.txt", "./s21_grep -in 123 test.txt");
  test("Test chiv flag", "grep -chiv 123 test.txt",
       "./s21_grep -chiv 123 test.txt");
  test("Test lsih flag", "grep -lsih hello test.txt",
       "./s21_grep -lsih hello test.txt");
  test("Test shvol flag", "grep -shvol hello test.txt",
       "./s21_grep -shvol hello test.txt");
  test("Test shvol flag", "grep -ovinl hello test.txt",
       "./s21_grep -ovinl hello test.txt");

  remove("test.txt");

  return 0;
}