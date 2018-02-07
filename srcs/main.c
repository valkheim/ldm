#include <signal.h>
#include "main.h"
#include "window.h"
#include "events.h"
#include "keyboard.h"
#include "x.h"
#include "args.h"

xcb_connection_t *c;
xcb_screen_t *screen;

static void display_screen_infos(void)
{
  printf ("\n");
  printf ("Informations of screen %u:\n", screen->root);
  printf ("  width.........: %d\n", screen->width_in_pixels);
  printf ("  height........: %d\n", screen->height_in_pixels);
  printf ("  white pixel...: %u\n", screen->white_pixel);
  printf ("  black pixel...: %u\n", screen->black_pixel);
  printf ("\n");
}

int main(int const argc, char **argv)
{
  t_args *args;

  init_args(&args, argc, argv);
  if (parse_args(argc, argv, args) == false)
    return (84);
  if (args->x == true)
    start_x_server(args);

  c = xcb_connect(NULL, NULL);
  if (xcb_connection_has_error(c) > 0)
  {
    fprintf(stderr, "xcb_connect failed\n");
    xcb_disconnect(c);
    return 1;
  }
  screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

  display_screen_infos();
  create_window();
  setup_keyboard();
  dm_event_loop();

  if (args->x == true)
    stop_x_server();
  xcb_disconnect(c);
  return 0;
}
