#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static pid_t x_server_pid;

void start_x_server(char const * const display, char const * const vt)
{
  if ((x_server_pid = fork()) == 0)
  {
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "/usr/bin/X %s %s", display, vt);
    execl("/bin/bash", "/bin/bash", "-c", cmd, NULL);
    printf("Failed to start X server");
    exit(1);
  }
  else
  {
    sleep(1);
  }
}

void stop_x_server(void)
{
  if (x_server_pid != 0)
    kill(x_server_pid, SIGKILL);
}

void sig_handler(/*int signo*/)
{
  stop_x_server();
}
