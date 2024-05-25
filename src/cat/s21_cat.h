#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct Flags {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int count;
} flags;

const struct option long_options[] = {{"number-nonblank", 0, NULL, 'b'},
                                      {"number", 0, NULL, 'e'},
                                      {"squeeze-blank", 0, NULL, 's'},
                                      {NULL, 0, NULL, 0}};

void flags_parser(int argc, char **argv, flags *flags);
void output(char *name_file, flags flags);
void output_line(char name_file[]);

#endif