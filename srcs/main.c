#include <signal.h>
#include "main.h"
#include "window.h"
#include "events.h"
#include "keyboard.h"
#include "x.h"

xcb_connection_t *c;
xcb_screen_t *screen;

int main(int argc, char **argv)
{
  char const *display = DISPLAY;
#ifndef DEBUG
  char const *vt = VT;
#endif
  if (argc == 3)
  {
    display = argv[1];
#ifndef DEBUG
    vt = argv[2];
#endif
  }
#ifndef DEBUG
  signal(SIGSEGV, sig_handler);
  signal(SIGTRAP, sig_handler);
  start_x_server(display, vt);
#endif
  setenv("DISPLAY", display, true);

  c = xcb_connect(NULL, NULL); // Callers need to use xcb_connection_has_error() to check for failure.
  if (xcb_connection_has_error(c) > 0)
  {
    fprintf(stderr, "xcb_connect failed\n");
    xcb_disconnect(c);
    return 1;
  }
  screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

  printf ("\n");
  printf ("Informations of screen %u:\n", screen->root);
  printf ("  width.........: %d\n", screen->width_in_pixels);
  printf ("  height........: %d\n", screen->height_in_pixels);
  printf ("  white pixel...: %u\n", screen->white_pixel);
  printf ("  black pixel...: %u\n", screen->black_pixel);
  printf ("\n");

  create_window();
  setup_keyboard();
  dm_event_loop();


  stop_x_server();
  xcb_disconnect(c);
  return 0;
}
