#pragma once

#define DISPLAY (":1")
#define VT ("vt01")

void start_x_server(char const * const display, char const * const vt);
void sig_handler(int signo);
void stop_x_server(void);
