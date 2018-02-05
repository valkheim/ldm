#include "main.h"
#include "window.h"

void create_window() {
  uint32_t mask;
  uint32_t values[2];

  xcb_window_t window;
  xcb_void_cookie_t cookie;

  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = screen->white_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;

  window = xcb_generate_id(c);
  cookie = xcb_create_window(c,
      XCB_COPY_FROM_PARENT, window, screen->root,
      0, 0, 640, 480,
      0,
      XCB_WINDOW_CLASS_INPUT_OUTPUT,
      screen->root_visual,
      mask, values);

  xcb_map_window(c, window);
}
