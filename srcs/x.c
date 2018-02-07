#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "args.h"
#include "x.h"

static pid_t x_server_pid;

void stop_x_server(void)
{
  if (x_server_pid != 0)
    kill(x_server_pid, SIGKILL);
}

static void sig_handler(/*int signo*/)
{
  stop_x_server();
}

void start_x_server(t_args *args)
{
  signal(SIGSEGV, sig_handler);
  signal(SIGTRAP, sig_handler);
  if ((x_server_pid = fork()) == 0)
  {
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "/usr/bin/X %s %s", args->display, args->vt);
    execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
    printf("Failed to start X server");
    exit(1);
  }
  else
  {
    sleep(1);
  }
  setenv("DISPLAY", args->display, true);
}
