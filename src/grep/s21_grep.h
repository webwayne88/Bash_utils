#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <locale.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX 1024

typedef struct Flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int count_fl;
  char *pattern[MAX];
  char *file[MAX];
  int count_pt;
  char lst_fl;
  int count_str;
  char *f_pattern[MAX];
} Flag;

typedef struct Found {
  char str[MAX];
  int num_str;
} Found;

Flag parser(int argc, char **argv);

void find(Flag flags, Found stuc[], char filename[], int flag);

void print(Flag flags, Found stuc[], char filename[], int flag);

char *lower(char *str);

int patern_in_string(char *patern[], char str[], Flag flags, int flag,
                     char filename[]);

int flag_s(char filename[]);

char flag_f(Flag flags, char *f_pattern[], char str[], int flag,
            char filename[]);

int flag_o(char *string, Flag flags, Found stuc[]);

#endif