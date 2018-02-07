#pragma once
#include <stdbool.h>
#include <getopt.h>

typedef struct s_args
{
  int ac;
  char **av;
  char *optarg;
  int optind;
  bool x;
  char *display;
  char *vt;
} t_args;

typedef bool (*t_args_func)(t_args *);
typedef struct  s_func_args
{
  char opt;
  t_args_func func;
} t_func_args;


bool args_usage(t_args *args);
bool args_x(t_args *args);
bool args_display(t_args *args);
bool args_virtual_terminal(t_args *args);

static __attribute__((__unused__)) t_func_args g_args_parsing_table[] = {
  {'h', args_usage},
  {'x', args_x},
  {'d', args_display},
  {'t', args_virtual_terminal},
  {0, NULL}
};

void init_args(t_args **args, int ac, char **av);
bool parse_args(int ac, char **av, t_args *args);