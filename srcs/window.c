#include "main.h"
#include "window.h"

void create_window()
{
  xcb_window_t const win = xcb_generate_id(c);
  uint32_t const mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t values[2];
  values[0] = screen->white_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
              XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
              XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
              XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE;
  xcb_create_window(c,                             /* Connection          */
                    XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                    win,                           /* window Id           */
                    screen->root,                  /* parent window       */
                    0, 0,                          /* x, y                */
                    150, 150,                      /* width, height       */
                    0,                            /* border_width        */
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                    screen->root_visual,           /* visual              */
                    mask, values);                      /* masks, values */
  xcb_map_window(c, win);
  xcb_flush(c);
}
