#pragma once
#include <xcb/xcb.h>
#include "event.h"

void create_window(void);
extern xcb_gcontext_t foreground;
extern xcb_drawable_t win;
