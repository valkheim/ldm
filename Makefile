all:
	gcc `pkg-config --libs xcb xcb-util` main.c events.c window.c -o dm && ./dm
