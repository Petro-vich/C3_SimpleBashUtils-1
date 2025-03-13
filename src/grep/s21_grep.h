#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Flags {
  bool e, i, v, c, l, n, h, s, f, o, no_flags;
  char *pattern;  // строка для хранения шаблонов
};
