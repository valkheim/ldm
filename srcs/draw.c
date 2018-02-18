/*
 * This file is part of the ldm distribution (https://github.com/valkheim/ldm)
 * Copyright (c) BORDER_WIDTH18 Charles Paulet.
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

#include "window.h"
#include "main.h"
#include "draw.h"

void draw(void)
{
  xcb_rectangle_t rectangles[] = {
  /* x, y, wdt, hgt */
    {0, 0, BORDER_WIDTH, screen->height_in_pixels},
    {screen->width_in_pixels - BORDER_WIDTH, 0, BORDER_WIDTH, screen->height_in_pixels},
    {0, 0, screen->width_in_pixels, BORDER_WIDTH},
    {0, screen->height_in_pixels - BORDER_WIDTH, screen->width_in_pixels, BORDER_WIDTH},
  };

  xcb_poly_fill_rectangle(c, win, foreground, sizeof(rectangles) / sizeof(rectangles[0]), rectangles);
  xcb_image_text_8(c, 5, win, foreground, 40, 40, "hello");
  xcb_flush(c);
}
