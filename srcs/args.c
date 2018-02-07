#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include "args.h"

bool args_usage(t_args *args)
{
  printf("USAGE: %s\n", args->av[0]);
  puts("\thelp\t\tDisplay this help");
  puts("\txoff\t\tDo not init X server");
  puts("\tdisplay\t\tSet display (:1 by default)");
  puts("\tvt\t\tSet virtual terminal (vt01 by default)");
  return (false);
}

bool args_x(t_args *args)
{
  args->x = false;
  return (true);
}

bool args_display(t_args *args)
{
  args->display = args->optarg;
  return (true);
}

bool args_virtual_terminal(t_args *args)
{
  args->vt = args->optarg;
  return (true);
}

bool parse_args(int ac, char **av, t_args *args)
{
  int opt;
  size_t i;
  bool ret = true;

  struct option long_opt[] = {
    {"help", no_argument, NULL, 'h'},
    {"xoff", no_argument, NULL, 'x'},
    {"display", required_argument, NULL, 'd'},
    {"vt", required_argument, NULL, 't'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(ac, av, "hxd:t:", long_opt, NULL)) != -1)
  {
    i = 0;
    while (i < (sizeof(g_args_parsing_table) / sizeof(*g_args_parsing_table)))
    {
      args->optarg = optarg;
      args->optind = optind;
      if (opt == g_args_parsing_table[i].opt)
        if (((*g_args_parsing_table[i].func)(args)) == false)
          ret = false;
      i++;
    }
  }
  return (ret);
}

void init_args(t_args **args, int ac, char **av)
{
  if ((*args = malloc(sizeof(**args))) == NULL)
    errx(84, "malloc failed : %s\n", strerror(errno));
  (*args)->ac = ac;
  (*args)->av = av;
  (*args)->x = true;
  (*args)->display = ":1";
  (*args)->vt = "vt01";
}
