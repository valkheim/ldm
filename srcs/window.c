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

#include "main.h"
#include "window.h"

xcb_gcontext_t foreground;
xcb_drawable_t win;

static xcb_gcontext_t create_graphical_ctx(uint32_t const color)
{
  /* create graphic context */
  xcb_gcontext_t const ctx = xcb_generate_id(c);
  uint32_t const mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  //values[0] = screen->white_pixel;
  uint32_t const values[] = { color, 0 };
  xcb_create_gc(c, ctx, win, mask, values);
  return ctx;
}

void create_window(void)
{
  uint32_t mask = 0;
  uint32_t values[2] = { 0 };

  win = screen->root;

  foreground = create_graphical_ctx(0xA349A3);
  /* create the window */
  win = xcb_generate_id(c);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = screen->black_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS   |
              XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
              XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
              XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE;
  xcb_create_window(c,                             /* Connection          */
                    XCB_COPY_FROM_PARENT,          /* depth (same as root)*/
                    win,                           /* window Id           */
                    screen->root,                  /* parent window       */
                    0, 0,                          /* x, y                */
                    screen->width_in_pixels,       /* width               */
                    screen->height_in_pixels,      /* height              */
                    0,                             /* border_width        */
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                    screen->root_visual,           /* visual              */
                    mask, values);                 /* masks, values       */
  xcb_map_window(c, win);
  xcb_flush(c);
}
