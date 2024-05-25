#include "s21_cat.h"

int main(int argc, char** argv) {
  flags flags = {0};
  flags_parser(argc, argv, &flags);
  if (flags.count > 0) {
    for (int i = flags.count; i < argc; i++) {
      output(argv[i], flags);
    }
  } else {
    for (int i = 1; i < argc; i++) {
      output_line(argv[i]);
    }
  }
  return 0;
}

void flags_parser(int argc, char** argv, flags* flags) {
  int c = 0;
  while ((c = getopt_long(argc, argv, "+beEnstTv", long_options, NULL)) != -1) {
    if (c == 'b') {
      flags->b = 1;
      flags->count++;
    } else if (c == 'e') {
      flags->e = 1;
      flags->v = 1;
      flags->count++;
    } else if (c == 'E') {
      flags->e = 1;
      flags->count++;
    } else if (c == 'n') {
      flags->n = 1;
      flags->count++;
    } else if (c == 's') {
      flags->s = 1;
      flags->count++;
    } else if (c == 't') {
      flags->t = 1;
      flags->v = 1;
      flags->count++;
    } else if (c == 'T') {
      flags->t = 1;
      flags->count++;
    } else if (c == 'v') {
      flags->v = 1;
      flags->count++;
    }
  }
}

void output(char* filename, flags flags) {
  FILE* file = fopen(filename, "r");
  if (file != NULL) {
    int ch = fgetc(file), pch = '\n', count = 1;
    while (ch != EOF) {
      if (flags.b == 1 && pch == '\n' && ch != '\n')
        printf("%6d\t", count++);
      else if (flags.n == 1 && flags.b == 0 && pch == '\n' && ch)
        printf("%6d\t", count++);
      if (ch == '\t' && flags.t == 1) {
        printf("^I");
      } else if (ch == '\n' && flags.e == 1) {
        if (flags.b && pch == '\n')
          printf("      \t$\n");
        else
          printf("$\n");
      } else if (flags.v == 1 && ch != '\n' && ch != '\t') {
        if (ch >= 0 && ch <= 31) {
          printf("^%c", ch + 64);
        } else if (ch == 127) {
          printf("^%c", ch - 64);
        } else
          printf("%c", ch);
      } else
        printf("%c", ch);
      int ppch = pch;
      pch = ch;
      ch = fgetc(file);
      while (flags.s == 1 && ch == '\n' && pch == '\n' && ppch == '\n') {
        ppch = pch;
        pch = ch;
        ch = fgetc(file);
      }
    }
    fclose(file);
  }
}

void output_line(char filename[]) {
  FILE* file = fopen(filename, "rt");
  if (file != NULL) {
    int ch = fgetc(file);
    while (ch != EOF) {
      putc(ch, stdout);
      ch = fgetc(file);
    }
    fclose(file);
  }
}