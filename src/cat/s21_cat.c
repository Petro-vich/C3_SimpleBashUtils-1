#include "s21_cat.h"

void FlagPars(int argc, char *argv[], struct Flags *flag) {
  int opt;
  while ((opt = getopt_long(argc, argv, "benstETv", long_options, NULL)) !=
         -1) {
    switch (opt) {
      case 'b':
        flag->b = true;
        break;
      case 'e':
        flag->e = true;
        flag->v = true;
        break;
      case 'n':
        flag->n = true;
        break;
      case 's':
        flag->s = true;
        break;
      case 't':
        flag->t = true;
        flag->v = true;
        break;
      case 'E':
        flag->E = true;
        break;
      case 'T':
        flag->T = true;
        break;
      case 'v':
        flag->v = true;
        break;
      default:
        exit(EXIT_FAILURE);
    }
  }
}

void process_and_print_char(const struct Flags *flags, unsigned char uc,
                            int *line_number_printed) {
  if (uc == '\t') {
    if (flags->t || flags->T) {
      printf("^I");
    } else {
      putchar(uc);
    }
  } else if (uc == '\n') {
    if (flags->e || flags->E) {
      printf("$\n");
      (*line_number_printed) = 0;
    } else {
      putchar(uc);
      (*line_number_printed) = 0;
    }
  } else if (flags->v) {
    if (uc < 32 && uc != '\t' && uc != '\n') {
      printf("^%c", uc + 64);
    } else if (uc == 127) {
      printf("^?");
    } else if (uc > 127 && uc < 160) {
      printf("M-^%c", uc - 64);
    } else if (uc >= 160 && uc < 255) {
      printf("M-%c", uc - 128);
    } else if (uc == 255) {
      printf("M-^?");
    } else {
      putchar(uc);
    }
  } else {
    putchar(uc);
  }
}

void process_file(const char *filename, struct Flags *flags) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror(filename);
    return;
  }

  int empty_lines = 0;  // count empty lines
  int line_number_printed = 0;

  int c;
  while ((c = fgetc(file)) != EOF) {
    unsigned char uc = (unsigned char)c;

    if (uc == '\n') {
      if (flags->s && empty_lines >= 1) {
        empty_lines++;
        if (empty_lines > 2) {
          continue;
        }
      } else {
        empty_lines = 1;
      }
    } else {
      empty_lines = 0;
    }

    if (!line_number_printed) {
      if (flags->b && uc != '\n') {
        printf("%6d\t", flags->line_number++);  // Number non-empty lines
        line_number_printed = 1;
      } else if (flags->n && !flags->b) {
        printf("%6d\t", flags->line_number++);  // Number all lines
        line_number_printed = 1;
      }
    }

    process_and_print_char(flags, uc, &line_number_printed);

    if (uc == '\n') {
      line_number_printed = 0;
    }
  }

  fclose(file);
}

int main(int argc, char *argv[]) {
  struct Flags flag = {0};
  flag.line_number = 1;

  FlagPars(argc, argv, &flag);

  if (optind >= argc) {
    fprintf(stderr, "use: %s [options] [file ...]\n", argv[0]);
    return EXIT_FAILURE;
  }

  for (int i = optind; i < argc; i++) {
    process_file(argv[i], &flag);
  }
}