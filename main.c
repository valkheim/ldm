// gcc `pkg-config --libs xcb` main.c -o main
#include "main.h"
#include "window.h"
#include "events.h"

xcb_connection_t *c;
xcb_screen_t *screen;

int main(void) {
  c = xcb_connect(NULL, NULL); // Callers need to use xcb_connection_has_error() to check for failure.
  screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

  create_window();

  xcb_flush(c);

  dm_event_loop();

  return EXIT_SUCCESS;
}
