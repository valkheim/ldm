// gcc `pkg-config --libs xcb` main.c -o main
#include "main.h"
#include "window.h"
#include "events.h"

xcb_connection_t *c;
xcb_screen_t *screen;

int main(void)
{
  c = xcb_connect(NULL, NULL); // Callers need to use xcb_connection_has_error() to check for failure.
  screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

  printf ("\n");
  printf ("Informations of screen %u:\n", screen->root);
  printf ("  width.........: %d\n", screen->width_in_pixels);
  printf ("  height........: %d\n", screen->height_in_pixels);
  printf ("  white pixel...: %u\n", screen->white_pixel);
  printf ("  black pixel...: %u\n", screen->black_pixel);
  printf ("\n");

  create_window();

  dm_event_loop();

  xcb_disconnect(c);

  return 0;
}
