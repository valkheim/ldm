#pragma once
#include <stdbool.h>

extern struct xkb_state *xkb_state;
extern struct xkb_compose_state *xkb_compose_state;
void setup_keyboard(void);
bool load_keymap(void);
