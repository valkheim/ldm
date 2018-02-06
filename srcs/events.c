#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include "main.h"
#include "events.h"

static void print_modifiers(xcb_keycode_t detail, uint32_t state)
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

static xcb_keysym_t xcb_get_keysym(xcb_keycode_t detail, uint16_t state)
{
  /*
   * adapt keysym using detail (modifier)
   * https://github.com/Cloudef/monsterwm-xcb/blob/master/monsterwm.c
   * https://lists.freedesktop.org/archives/xcb/2010-February/005562.html
   * https://github.com/ehntoo/unagi/blob/master/src/key.c
   * /usr/include/xcb/xproto.h:500
   */
  xcb_key_symbols_t *keysyms;
  xcb_keysym_t k0, k1;

  if (!(keysyms = xcb_key_symbols_alloc(c)))
    return 0;

  if (state & XCB_MOD_MASK_5) {
    k0 = xcb_key_symbols_get_keysym(keysyms, detail, 4);
    k1 = xcb_key_symbols_get_keysym(keysyms, detail, 5);
  } else {
    k0 = xcb_key_symbols_get_keysym(keysyms, detail, 0);
    k1 = xcb_key_symbols_get_keysym(keysyms, detail, 1);
  }

  xcb_key_symbols_free(keysyms);

  if (k1 == XCB_NO_SYMBOL)
    k1 = k0;

  if (xcb_is_keypad_key(k1))
  {
    /* The Shift modifier  is on, or if the Lock  modifier is on and
     * is interpreted as ShiftLock, use the first KeySym */
    if ((state & XCB_MOD_MASK_SHIFT) || (state & XCB_MOD_MASK_LOCK)) {
      return k0;
    } else {
      return k1;
    }
    /* The Shift and Lock modifers are both off, use the first KeySym */
  } else if (!(state & XCB_MOD_MASK_SHIFT) && !(state & XCB_MOD_MASK_LOCK)) {
    return k0;
    /* The Shift  modifier is  off and  the Lock modifier  is on  and is
     * interpreted as CapsLock */
  } else if (!(state & XCB_MOD_MASK_SHIFT) && (state & XCB_MOD_MASK_LOCK)) {
    /* The  first Keysym  is  used  but if  that  KeySym is  lowercase
     * alphabetic,  then the  corresponding uppercase  KeySym  is used
     * instead */
    return k1;
    /* The Shift modifier is on, and the Lock modifier is on and is
     * interpreted as CapsLock */
  } else if((state & XCB_MOD_MASK_SHIFT) && (state & XCB_MOD_MASK_LOCK)) {
    /* The  second Keysym  is used  but  if that  KeySym is  lowercase
     * alphabetic,  then the  corresponding uppercase  KeySym  is used
     * instead */
    return k1;
    /* The  Shift modifer  is on,  or  the Lock  modifier is  on and  is
     * interpreted as ShiftLock, or both */
  } else if((state & XCB_MOD_MASK_SHIFT) || (state & XCB_MOD_MASK_LOCK)) {
    return k1;
  }
  return XCB_NO_SYMBOL;
}

static void key_press_management(xcb_key_press_event_t *ev)
{
  xcb_keysym_t keysym;

  print_modifiers(ev->detail, ev->state);
  keysym = xcb_get_keysym(ev->detail, ev->state);
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
        printf("event = %s\n",xcb_event_get_label(e->response_type));
    }
    free(e);
  }
}
