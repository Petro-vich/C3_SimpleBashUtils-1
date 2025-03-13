#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                {"number", no_argument, NULL, 'n'},
                                {"squeeze-blank", no_argument, NULL, 's'},
                                {NULL, 0, NULL, 0}};

struct Flags {
  bool b, e, n, s, t;
  bool E;
  bool T;
  bool v;
  int line_number;
};