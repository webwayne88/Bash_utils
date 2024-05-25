#include "s21_grep.h"

int main(int argc, char** argv) {
  Flag flags = parser(argc, argv);
  Found stuc[MAX];
  int count_fe = 0;
  int flag = 0;
  if (flags.e == 0 && flags.f == 0) {
    char* res;
    int count = 1;
    while ((res = strstr(argv[count], "-")) != NULL) {
      count++;
    }
    flags.pattern[flags.count_pt] = argv[count];
    flags.count_pt++;
  }
  if (flags.e + flags.f > 1) flags.count_fl += flags.e + flags.f - 1;
  for (int i = flags.count_fl + 2; i < argc; i++) {
    flags.file[count_fe] = argv[i];
    count_fe++;
  }
  if (count_fe > 1) flag = 1;
  if (flags.h) flag = 0;
  for (int i = flags.count_fl + 2; i < argc; i++) {
    if (flag_s(argv[i]) && flags.s) {
      find(flags, stuc, argv[i], flag);
    } else if (!flag_s(argv[i]) && !flags.s) {
      printf("grep: %s: No such file or directory\n", argv[i]);
    } else if (flag_s(argv[i]), !flags.s) {
      find(flags, stuc, argv[i], flag);
    }
  }
  if (flags.v && !flags.c && !flags.l && flag) printf("\n");
  return 0;
}

Flag parser(int argc, char** argv) {
  int value;
  const struct option long_options[] = {{NULL, 0, NULL, 0}};
  Flag flags = {0};
  while ((value = getopt_long(argc, argv, "e:ivclnhsf:o", long_options,
                              NULL)) != -1) {
    if (value == 'e') {
      flags.pattern[flags.count_pt] = optarg;
      flags.e++, flags.count_pt++, flags.count_fl++;
    } else if (value == 'i') {
      flags.i = 1, flags.count_fl++;
    } else if (value == 'v') {
      flags.v = 1, flags.count_fl++;
    } else if (value == 'c') {
      flags.c = 1, flags.count_fl++;
    } else if (value == 'l') {
      flags.l = 1, flags.count_fl++;
    } else if (value == 'n') {
      flags.n = 1, flags.count_fl++;
    } else if (value == 'h') {
      flags.h = 1, flags.count_fl++;
    } else if (value == 's') {
      flags.s = 1, flags.count_fl++;
    } else if (value == 'f') {
      flags.f_pattern[flags.f] = optarg;
      flags.f++, flags.count_fl++;
    } else if (value == 'o') {
      flags.o = 1, flags.count_fl++;
    }
  }
  for (int i = 1; i < argc; i++) {
    if ((strstr(argv[argc - i], "-") != NULL) && strlen(argv[argc - i]) > 2) {
      flags.count_fl -= strlen(argv[argc - i] - 2);
    }
  }
  return flags;
}

void find(Flag flags, Found stuc[], char filename[], int flag) {
  FILE* file = fopen(filename, "rt");
  int num = 1;
  char str_org[MAX];
  char str[MAX];
  int res;
  while (fgets(str, MAX, file) != NULL) {
    strcpy(str_org, str);
    if (flags.i) {
      lower(str);
    }
    if (flags.o) {
      int V = flag_o(str, flags, stuc);
      if (V && !flags.v) {
        stuc[flags.count_str].num_str = num;
        flags.count_str++;
      } else if (!V && flags.v) {
        strcpy(stuc[flags.count_str].str, str);
        stuc[flags.count_str].num_str = num;
        flags.count_str++;
      }
    }
    int res_2 = 0;
    if (flags.f) {
      res_2 = flag_f(flags, flags.f_pattern, str, flag, filename);
    }
    if (!flags.o) {
      res = patern_in_string(flags.pattern, str, flags, flag, filename);
      if (res && !flags.v && !flags.o) {
        strcpy(stuc[flags.count_str].str, str_org);
        stuc[flags.count_str].num_str = num;
        flags.count_str++;
      } else if (!res && flags.v && !flags.o) {
        strcpy(stuc[flags.count_str].str, str_org);
        stuc[flags.count_str].num_str = num;
        flags.count_str++;
      } else if (res_2 && !flags.v && !flags.o && flags.f) {
        strcpy(stuc[flags.count_str].str, str_org);
        stuc[flags.count_str].num_str = num;
        flags.count_str++;
      } else if (!res_2 && flags.v && !flags.o && flags.f) {
        strcpy(stuc[flags.count_str].str, str_org);
        stuc[flags.count_str].num_str = num;
        flags.count_str++;
      }
    }
    num++;
  }
  print(flags, stuc, filename, flag);
  fclose(file);
}

void print(Flag flags, Found stuc[], char filename[], int flag) {
  if (flags.c && flag == 1 && !flags.l) {
    printf("%s:%d\n", filename, flags.count_str);
  } else if (flags.c && flag == 0 && !flags.l) {
    printf("%d\n", flags.count_str);
  } else if (flags.c && flag == 0 && flags.l) {
    printf("%d\n", flags.count_str > 0);
  } else if (flags.c && flag == 1 && flags.l) {
    printf("%s:%d\n", filename, flags.count_str > 0);
  }
  if (flags.l && flags.count_str > 0) {
    printf("%s\n", filename);
  }
  if (flags.n && !flags.c && !flags.l && !flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%d:", stuc[i].num_str);
      printf("%s", stuc[i].str);
    }
  } else if (flags.n && !flags.c && !flags.l && flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%s:", filename);
      printf("%d:", stuc[i].num_str);
      printf("%s", stuc[i].str);
    }
  }
  if (!flags.c && !flags.l && !flags.n && flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%s:%s", filename, stuc[i].str);
    }
  } else if (!flags.c && !flags.l && !flags.n && !flag) {
    for (int i = 0; i < flags.count_str; i++) {
      printf("%s", stuc[i].str);
    }
  }
}

char* lower(char* str) {
  for (unsigned long i = 0; i < strlen(str); i++) {
    char ch = str[i];
    if (ch >= 65 && ch <= 90) {
      ch = ch + 32;
    }
    str[i] = ch;
  }
  return str;
}

int patern_in_string(char* patern[], char str[], Flag flags, int flag,
                     char filename[]) {
  int i = 0, res = 0;
  while (patern[i] != NULL) {
    regex_t regex;
    if (flags.i) patern[i] = lower(patern[i]);
    const char* pattern = patern[i];
    regcomp(&regex, pattern, REG_EXTENDED);
    if (!regexec(&regex, str, 0, 0, 0)) {
      res = 1;
      if (flags.o && !flag) {
        printf("%s\n", patern[i]);
      } else if (flags.o && flag) {
        printf("%s:%s\n", filename, patern[i]);
      }
    }
    i++;
    regfree(&regex);
  }
  return res;
}

int flag_s(char filename[]) {
  FILE* f = fopen(filename, "rt");
  if (f == NULL) return 0;
  fclose(f);
  return 1;
}

char flag_f(Flag flags, char* f_pattern[], char str[], int flag,
            char filename[]) {
  int i = 0, res = 0;
  while (f_pattern[i] != NULL) {
    FILE* file = fopen(f_pattern[i], "rt");
    char stra[MAX];
    while (fgets(stra, MAX, file) != NULL) {
      regex_t regex;
      if (flags.i) lower(stra);
      const char* pattern = stra;
      regcomp(&regex, pattern, 0);
      if (!regexec(&regex, str, 0, 0, 0)) {
        res = 1;
        if (flags.o && !flag) {
          printf("%s\n", stra);
        } else if (flags.o && flag) {
          printf("%s:%s\n", filename, stra);
        }
      }
      regfree(&regex);
    }
    fclose(file);
    i++;
  }
  return res;
}

int flag_o(char* string, Flag flags, Found stuc[]) {
  char* ps;
  int i = 0;
  int flag = 0;
  int flag_2 = 0;
  setlocale(LC_ALL, "");
  ps = string;
  while (flags.pattern[i] != NULL) {
    int eflag;
    int status;
    regex_t regex;
    regmatch_t pmatch[100];
    regcomp(&regex, flags.pattern[i], REG_EXTENDED);
    eflag = 0;
    while ((status = regexec(&regex, ps, 1, pmatch, eflag)) == 0) {
      if (!flags.v) {
        if (flag_2 == 0) {
          strcpy(stuc[flags.count_str].str, flags.pattern[i]);
          strcat(stuc[flags.count_str].str, "\n");
          flag_2 = 1;
        } else {
          strcat(stuc[flags.count_str].str, flags.pattern[i]);
          strcat(stuc[flags.count_str].str, "\n");
        }
      }
      ps += pmatch[0].rm_eo;
      eflag = REG_NOTBOL;
      flag = 1;
    }
    regfree(&regex);
    i++;
  }
  return flag;
}