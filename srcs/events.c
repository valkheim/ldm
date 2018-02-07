#include <xkbcommon/xkbcommon-compose.h>
#include "main.h"
#include "events.h"
#include "keyboard.h"

static void print_modifiers(xcb_keycode_t const detail, uint32_t state)
{
  const char **mod, *mods[] = {
    "Shift", "Lock", "Ctrl", "Alt",
    "Mod2", "Mod3", "Mod4", "Mod5",
    "Button1", "Button2", "Button3", "Button4", "Button5"
  };
  printf("xcb: keypress: code: %d mod: %d (", detail, state);
  for (mod = mods ; state; state >>= 1, mod++)
    if (state & 1)
      printf("%s", *mod);
  puts(")");
}

static bool handle_control_keysym(xkb_keysym_t const ksym)
{
  switch (ksym) {
    case XKB_KEY_Left:
      puts("LEFT");
      return true;
    case XKB_KEY_Up:
      puts("UP");
      return true;
    case XKB_KEY_Right:
      puts("RIGHT");
      return true;
    case XKB_KEY_Down:
      puts("DOWN");
      return true;
    case XKB_KEY_Return:
      puts("RETURN");
      return true;
    case XKB_KEY_BackSpace:
      puts("BACKSPACE");
      return true;
    default:
      return false;
  }
}

static void xkb_get_keysym(xcb_keycode_t const detail)
{
  char buffer[128];
  bool composed = false;
  int n;

  xkb_keysym_t ksym = xkb_state_key_get_one_sym(xkb_state, detail);
  if (handle_control_keysym(ksym))
    return;
  if (xkb_compose_state && xkb_compose_state_feed(xkb_compose_state, ksym) == XKB_COMPOSE_FEED_ACCEPTED) {
    switch (xkb_compose_state_get_status(xkb_compose_state)) {
      case XKB_COMPOSE_NOTHING:
        break;
      case XKB_COMPOSE_COMPOSING:
        return;
      case XKB_COMPOSE_COMPOSED:
        /* xkb_compose_state_get_utf8 doesn't include the terminating byte in the return value
         * as xkb_keysym_to_utf8 does. Adding one makes the variable n consistent. */
        n = xkb_compose_state_get_utf8(xkb_compose_state, buffer, sizeof(buffer)) + 1;
        ksym = xkb_compose_state_get_one_sym(xkb_compose_state);
        composed = true;
        break;
      case XKB_COMPOSE_CANCELLED:
        xkb_compose_state_reset(xkb_compose_state);
        return;
    }
  }

  if (!composed) {
    n = xkb_keysym_to_utf8(ksym, buffer, sizeof(buffer));
  }

  if (n < 2)
    return;

  printf("xkb buffer : %s\n", buffer);
}

static void key_press_management(xcb_key_press_event_t const * const ev)
{
  print_modifiers(ev->detail, ev->state);
  xkb_get_keysym(ev->detail);
}

void event_management(xcb_generic_event_t const * const event)
{
  uint8_t response_type = XCB_EVENT_RESPONSE_TYPE(event);

  if (response_type == 0)
  {
    perror("response_type = 0");
    return;
  }

  switch(response_type)
  {
    case XCB_KEY_PRESS:
      if (!load_keymap())
        fprintf(stderr, "Could not load keymap");
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
        printf("event = %s\n",xcb_event_get_label(e->response_type));
        event_management(e);
    }
    free(e);
  }
}
