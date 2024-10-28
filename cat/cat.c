#include "cat.h"

#define LEN 256
#define MAX_LINE_LEN 1024

int main(int argc, char *argv[]) {
  if (argc < 2) printf("Нет переднных аргументов\n");

  CatOption options = {0};
  options.start_index = 1;
  options.flag_enable = false;

  parser(argc, argv, &options);

  for (int i = options.start_index; i < argc; i++) {
    FILE *file = openFile(argv[i], "r+");
    if (file == NULL) continue;

    if (options.flag_enable) {
      FILE *temp = openFile("temp.txt", "w");

      while ((options.ch = fgetc(file)) != EOF) {
        fputc(options.ch, temp);
      }

      temp = freopen("temp.txt", "r", temp);

      if (options.s_flag == 1) s_Flag(temp);
      if (options.b_flag == 1) bFlag(temp);
      if (options.v_flag == 1) vFlag(temp);
      if (options.e_flag == 1) eFlag(temp);
      if (options.n_flag == 1 && options.b_flag == 0) nFlag(temp);
      if (options.t_flag == 1) tFlag(temp, options.b_flag, options.n_flag);

      temp = freopen("temp.txt", "r", temp);
      readFile(temp);
      fclose(temp);
    } else
      readFile(file);

    fclose(file);
    remove("temp.txt");
  }

  return 0;
}

void parser(int argc, char *argv[], CatOption *options) {
  if (argc > 2) {
    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-b") == 0 ||
          strcmp(argv[i], "--number-nonblank") == 0) {
        options->b_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }
      if (strcmp(argv[i], "-e") == 0) {
        options->e_flag = 1;
        options->v_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }
      if (strcmp(argv[i], "-E") == 0 && strcmp(argv[i], "-e") != 0) {
        options->e_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }

      if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--number") == 0) {
        options->n_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }
      if (strcmp(argv[i], "-s") == 0 ||
          strcmp(argv[i], "--squeeze-blank") == 0) {
        options->s_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }
      if (strcmp(argv[i], "-t") == 0) {
        options->t_flag = 1;
        options->v_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }
      if (strcmp(argv[i], "-T") == 0 && strcmp(argv[i], "-t") != 0) {
        options->t_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }
      if (strcmp(argv[i], "-v") == 0) {
        options->v_flag = 1;
        options->start_index++;
        options->flag_enable = true;
      }
    }
  }
}

FILE *openFile(char *path, char *type) {
  FILE *file = fopen(path, type);
  if (file == NULL) printf("Невозможно открыть файл: %s\n", path);

  return file;
}

void readFile(FILE *file) {
  char line[MAX_LINE_LEN];
  while (fgets(line, sizeof(line), file)) {
    printf("%s", line);
  }
}

void copyFile(FILE *main_file, FILE *temp_file) {
  rewind(temp_file);
  main_file = freopen("temp.txt", "w", main_file);

  int ch;
  while ((ch = fgetc(temp_file)) != EOF) {
    fputc(ch, main_file);
  }

  fclose(temp_file);

  rewind(main_file);
  main_file = freopen("temp.txt", "r", main_file);
}

void bFlag(FILE *file) {
  FILE *temp_file = tmpfile();

  char line[MAX_LINE_LEN];
  int line_number = 1;
  while (fgets(line, sizeof(line), file)) {
    if (line[0] != '\0' && line[0] != '\n') {
      fprintf(temp_file, "%6d\t%s", line_number, line);
      line_number++;
    } else {
      fprintf(temp_file, "%s", line);
    }
  }

  copyFile(file, temp_file);
}

void vFlag(FILE *file) {
  FILE *temp_file = tmpfile();

  int c;
  while ((c = fgetc(file)) != EOF) {
    if (c == '\n' || c == '\t') {
      fputc(c, temp_file);
    } else if (c == 0) {
      fprintf(temp_file, "^@");
    } else if (c >= 1 && c <= 31) {
      fprintf(temp_file, "^%c", c + 'A' - 1);
    } else if (c == 127) {
      fprintf(temp_file, "^?");
    } else {
      fputc(c, temp_file);
    }
  }

  copyFile(file, temp_file);
}

void eFlag(FILE *file) {
  FILE *temp_file = tmpfile();

  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') fputc('$', temp_file);
    fputc(ch, temp_file);
  }

  copyFile(file, temp_file);
}

void nFlag(FILE *file) {
  FILE *temp_file = tmpfile();

  char line[MAX_LINE_LEN];
  int line_number = 1;
  while (fgets(line, sizeof(line), file)) {
    fprintf(temp_file, "%6d\t%s", line_number++, line);
  }

  copyFile(file, temp_file);
}

void s_Flag(FILE *file) {
  FILE *temp_file = tmpfile();

  char line[MAX_LINE_LEN];
  int blank_lines = 1;

  while (fgets(line, sizeof(line), file)) {
    if (line[0] == '\n' || (strspn(line, " \t\n\r") == strlen(line))) {
      if (blank_lines == 1)
        blank_lines++;
      else
        continue;
    } else
      blank_lines = 1;

    fprintf(temp_file, "%s", line);
  }

  copyFile(file, temp_file);
}

void tFlag(FILE *file, int b_flag, int n_flag) {
  FILE *temp_file = tmpfile();

  char ch;
  int first_tab = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\t') {
      if (!first_tab && (b_flag || n_flag)) {
        first_tab = 1;
        putchar(ch);
        continue;
      }
      printf("^I");
    } else {
      putchar(ch);
      if (ch == '\n') first_tab = 0;
    }
  }

  copyFile(file, temp_file);
}