#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include "main.h"
#include "events.h"

static void print_modifiers (uint32_t mask)
{
  const char **mod, *mods[] = {
    "Shift", "Lock", "Ctrl", "Alt",
    "Mod2", "Mod3", "Mod4", "Mod5",
    "Button1", "Button2", "Button3", "Button4", "Button5"
  };
  printf ("Modifier mask: ");
  for (mod = mods ; mask; mask >>= 1, mod++)
    if (mask & 1)
      printf("%s", *mod);
  putchar ('\n');
}

static xcb_keysym_t xcb_get_keysym(xcb_keycode_t detail) {
    xcb_key_symbols_t *keysyms;
    xcb_keysym_t keysym;

    if (!(keysyms = xcb_key_symbols_alloc(c)))
      return 0;

    keysym = xcb_key_symbols_get_keysym(keysyms, detail, 0);

    /*
     * adapt keysym using detail (modifier)
     * https://github.com/Cloudef/monsterwm-xcb/blob/master/monsterwm.c
     * https://lists.freedesktop.org/archives/xcb/2010-February/005562.html
     */

    xcb_key_symbols_free(keysyms);
    return keysym;
}

static void key_press_management(xcb_key_press_event_t *event)
{
  xcb_key_press_event_t *ev = (xcb_key_press_event_t *)event;
  xcb_keysym_t keysym = xcb_get_keysym(ev->detail);
  print_modifiers(ev->state);
  printf("xcb: keypress: code: %d mod: %d\n", ev->detail, ev->state);
  printf("xcb: keysym: %c\n", keysym);
}

void event_management(xcb_generic_event_t *event)
{
  uint8_t response_type = XCB_EVENT_RESPONSE_TYPE(event);

  if (response_type == 0)
  {
    /* This is an error, not a event */
    perror("response_type = 0");
    return;
  }

  switch(response_type)
  {
    case XCB_KEY_PRESS:
      key_press_management((void *)event);
      break;
    default:
      //printf("event = %s\n",xcb_event_get_label(event->response_type));
      //printf("%d\n",response_type);
      //perror("this kind of event is not managed\n");
      break;
  }
}

void dm_event_loop()
{
  xcb_generic_event_t *e;
  int done;

  /*enter the main loop*/
  done = 0;
  while (!done && (e = xcb_wait_for_event(c)))
  {
    switch(e->response_type)
    {
      /*(re)draw the window*/
      case XCB_EXPOSE:
        printf ("EXPOSE\n");
        xcb_expose_event_t *ev = (xcb_expose_event_t *)e;
        printf ("Window %u exposed. Region to be redrawn at location (%d,%d), with dimension (%d,%d)\n",
            ev->window, ev->x, ev->y, ev->width, ev->height);
        break;
        //case XCB_KEY_PRESS:
        //  done = 1;
        //  break;
      default:
        event_management(e);
        printf("The events = %s\n",xcb_event_get_label(e->response_type));
    }
    free(e);
  }
}
