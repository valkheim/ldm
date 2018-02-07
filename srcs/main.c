/*
 * This file is part of the ldm distribution (https://github.com/valkheim/ldm)
 * Copyright (c) 2018 Charles Paulet.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
