#define _GNU_SOURCE

#include "grep.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Недостаточно аргументов");
    exit(1);
  }

  GrepOptions options = {0};
  options.start_index = 1;
  parse_arguments(argc, argv, &options);

  int file_count = argc - options.start_index;
  regex_t regex[MAX_PATTERNS];
  for (int i = 0; i < MAX_PATTERNS; i++) {
    regex_t r = {0};
    regex[i] = r;
  }
  for (int p = 0; p < options.pattern_count; p++) {
    int ret = regcomp(&regex[p], options.patterns[p],
                      REG_EXTENDED | (options.inverted ? REG_ICASE : 0) |
                          (options.ignore_case ? REG_ICASE : 0));
    if (ret) {
      fprintf(stderr, "Ошибка компиляции регулярного выражения\n");
      return 1;
    }
  }

  for (int i = options.start_index; i < argc; ++i) {
    char *file = argv[options.start_index++];
    FILE *input = fopen(file, "r");
    if (input == NULL) {
      if (!options.suppress_errors) perror("Error opening file");
      continue;
    }

    process(input, file, regex, file_count, &options);

    fclose(input);
  }

  for (int p = 0; p < options.pattern_count; p++) {
    regfree(&regex[p]);
  }

  if (options.f_flag) {
    for (int p = 0; p < options.pattern_count; p++) {
      free(options.patterns[p]);
    }
  }

  return 0;
}

void parse_arguments(int argc, char *argv[], GrepOptions *options) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      (options->start_index)++;
      for (int j = 1; argv[i][j] != '\0'; j++) {
        char flag = argv[i][j];
        if (flag == 'e') {
          ++(options->start_index);
          options->patterns[options->pattern_count++] = argv[++i];
          options->trigger = 1;
        } else if (flag == 'i')
          options->ignore_case = 1;
        else if (flag == 'v')
          options->inverted = 1;
        else if (flag == 'c')
          options->c_flag = 1;
        else if (flag == 'l')
          options->list_files = 1;
        else if (flag == 'n')
          options->show_line_numbers = 1;
        else if (flag == 'h')
          options->hide_filenames = 1;
        else if (flag == 's')
          options->suppress_errors = 1;
        else if (flag == 'f') {
          ++(options->start_index);
          read_patterns_from_file(argv[++i], options->patterns,
                                  &options->pattern_count);
          options->trigger = 1;
          options->f_flag = 1;
        } else if (flag == 'o')
          options->only_matching = 1;
        else
          fprintf(stderr, "Неизвестный флаг: -%c\n", flag);

        if (options->trigger) {
          options->trigger = 0;
          break;
        }
      }
    } else if (*options->patterns == NULL) {
      options->patterns[options->pattern_count++] = argv[i];
      options->start_index++;
    }
  }
}

void matching(char *line, regex_t *regex) {
  regmatch_t match = {0};
  int offset = 0;

  while (regexec(regex, line + offset, 1, &match, 0) == 0) {
    if (match.rm_so != -1 && match.rm_eo != -1) {
      offset += match.rm_so;
      printf("%.*s\n", (int)(match.rm_eo - match.rm_so), line + offset);
      offset += match.rm_eo;
    } else {
      break;
    }
  }
}

char *getLastSym(char *line) { return &line[strlen(line) - 1]; }

void print_flag(int *lTriggered, int line_number, char *line, char *file,
                regex_t *regex, int file_count, GrepOptions *options) {
  if (options->list_files) {
    printf("%s\n", file);
    *lTriggered = 1;
  } else if (!options->c_flag) {
    if (!options->hide_filenames && file_count > 1) printf("%s:", file);
    if (options->show_line_numbers) printf("%d:", line_number);
    if (options->only_matching)
      matching(line, regex);
    else {
      char *lastSym = getLastSym(line);
      if (*lastSym == '\n') *lastSym = '\0';
      printf("%s\n", line);
    }
  }
}

void process(FILE *input, char *file, regex_t *regex, int file_count,
             GrepOptions *options) {
  char line[MAX_LINE_LENGTH] = {0};
  size_t len = MAX_LINE_LENGTH;
  int line_number = 0;
  int count = 0;

  int lTriggered = 0;
  while (fgets(line, len, input) != NULL) {
    if (lTriggered) break;
    line_number++;
    int printed = 0;
    for (int i = 0; i < options->pattern_count; i++) {
      if ((regexec(&regex[i], line, 0, NULL, 0) == 0) ^ options->inverted) {
        if (printed || lTriggered) break;
        count++;
        printed = 1;
        print_flag(&lTriggered, line_number, line, file, regex, file_count,
                   options);
      }
    }
  }

  if (options->c_flag /*&& p + 1 == pattern_count */) {
    if (file_count > 1 && !options->hide_filenames) printf("%s:", file);
    printf("%d\n", count);
  }
}

void read_patterns_from_file(char *filename, char *patterns[],
                             int *pattern_count) {
  FILE *pattern_file = fopen(filename, "r");
  if (pattern_file == NULL) {
    fprintf(stderr, "Ошибка открытия файла с паттернами: %s\n", filename);
    exit(1);
  }

  char line[MAX_LINE_LENGTH];
  while (fgets(line, sizeof(line), pattern_file) != NULL) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }

    if (*pattern_count < MAX_PATTERNS) {
      char *copy_line = calloc(strlen(line) + 1, sizeof(char));
      strcpy(copy_line, line);
      patterns[(*pattern_count)++] = copy_line;
    } else {
      fprintf(stderr, "Превышено максимальное количество паттернов.\n");
      fclose(pattern_file);
      exit(1);
    }
  }

  fclose(pattern_file);
}
