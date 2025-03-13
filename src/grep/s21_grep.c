#include "s21_grep.h"

void AddPattern(struct Flags *flag, const char *new_pattern) {
  size_t old_length = flag->pattern ? strlen(flag->pattern) : 0;
  size_t new_length = strlen(new_pattern);
  size_t combined_length = old_length + new_length + 2;  // [ | ] + [ \0 ]

  char *combined_pattern = malloc(combined_length);
  if (!combined_pattern) {
    perror("error allocate memory");
    exit(EXIT_FAILURE);
  }

  if (flag->pattern) {
    snprintf(combined_pattern, combined_length, "%s|%s", flag->pattern,
             new_pattern);
    free(flag->pattern);
  } else {
    snprintf(combined_pattern, combined_length, "%s", new_pattern);
  }

  flag->pattern = combined_pattern;
}

void read_patterns_from_file(const char *filename, struct Flags *flag) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("error open file");
    exit(EXIT_FAILURE);
  }
  char line[1024];
  while (fgets(line, sizeof(line), file)) {
    line[strcspn(line, "\n")] = 0;
    AddPattern(flag, line);
  }
  fclose(file);
}

void FlagPars(int argc, char *argv[], struct Flags *flag, int *pattern_count,
              int *file_start) {
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        flag->e = true;
        AddPattern(flag, optarg);
        (*pattern_count)++;
        break;
      case 'i':
        flag->i = true;
        break;
      case 'v':
        flag->v = true;
        break;
      case 'c':
        flag->c = true;
        break;
      case 'l':
        flag->l = true;
        break;
      case 'n':
        flag->n = true;
        break;
      case 'h':
        flag->h = true;
        break;
      case 's':
        flag->s = true;
        break;
      case 'f':
        flag->f = true;
        read_patterns_from_file(optarg, flag);
        break;
      case 'o':
        flag->o = true;
        break;
      default:
        fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...\n");
        exit(EXIT_FAILURE);
    }
  }
  *file_start = optind;

  if (!flag->e && !flag->f && optind < argc) {
    flag->pattern = strdup(argv[optind++]);
    *file_start = optind;
    flag->no_flags = true;
  }

  if (!flag->pattern && *pattern_count == 0) {
    fprintf(stderr, "s21_grep: No pattern provided\n");
    exit(EXIT_FAILURE);
  }
}

bool RegularExp(struct Flags flag, char *line, regex_t *regex) {
  bool matched = false;
  if (regexec(regex, line, 0, NULL, 0) == 0) {
    matched = true;
  }
  if (flag.v) {
    matched = !matched;
  }
  return matched;
}

void realiz_flag_o(const struct Flags *flag, char *line, regex_t *regex,
                   int line_number, bool MultiFiles, const char *filename) {
  regmatch_t match;
  int offset = 0;

  while (regexec(regex, line + offset, 1, &match, 0) == 0) {
    if (MultiFiles && !flag->h) {
      printf("%s:", filename);
    }
    if (flag->n) {
      printf("%d:", line_number);
    }

    for (int i = match.rm_so; i < match.rm_eo; i++) {
      putchar(line[offset + i]);
    }

    putchar('\n');
    offset += match.rm_eo;
  }
}

void FlagImplement(char file[100][1024], int file_count, struct Flags flag) {
  char line[1024] = {0};
  int line_number = 0;
  int match_count = 0;
  bool MultiFiles = file_count > 1;

  regex_t regex;
  if (regcomp(&regex, flag.pattern, REG_EXTENDED | (flag.i ? REG_ICASE : 0)) !=
      0) {
    fprintf(stderr, "Regular expression compilation error: %s\n", flag.pattern);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < file_count; i++) {
    FILE *fp = fopen(file[i], "r");
    if (!fp) {
      if (!flag.s) {
        perror(file[i]);
      }
      continue;
    }

    while (fgets(line, sizeof(line), fp)) {
      line_number++;

      if (RegularExp(flag, line, &regex)) {
        match_count++;

        if (line[strlen(line) - 1] != '\n') {
          line[strlen(line)] = '\n';
          line[strlen(line) + 1] = '\0';
        }

        if (MultiFiles && !flag.h && !flag.l && !flag.c && !flag.o) {
          printf("%s:", file[i]);
        }

        if (flag.n && !flag.l && !flag.c && !flag.o) {
          printf("%d:", line_number);
        }

        if (!flag.l && !flag.c && !flag.o) {
          printf("%s", line);
        }

        if (flag.o && !flag.c && !flag.l) {
          realiz_flag_o(&flag, line, &regex, line_number, MultiFiles, file[i]);
        }
      }
      memset(line, 0, 1024);
    }

    if (flag.l && match_count > 0) {
      printf("%s\n", file[i]);
    }

    if (flag.c && !flag.l) {
      if (MultiFiles && !flag.h) {
        printf("%s:%d\n", file[i], match_count);
      } else {
        printf("%d\n", match_count);
      }
    }

    match_count = 0;
    line_number = 0;
    fclose(fp);
  }

  regfree(&regex);
}

int main(int argc, char *argv[]) {
  struct Flags flag = {0};
  int pattern_count = 0;
  int file_start = 0;
  char file[100][1024];
  int file_count = 0;

  FlagPars(argc, argv, &flag, &pattern_count, &file_start);

  for (int i = file_start; i < argc; i++) {
    strcpy(file[file_count++], argv[i]);
  }

  FlagImplement(file, file_count, flag);

  if (flag.pattern) {
    free(flag.pattern);
  }

  return 0;
}
