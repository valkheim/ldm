CC = gcc
RM = rm -rf
NAME = ldm
RULES = all clean fclean dbg re

D_OBJS = objs
D_SRCS = srcs
D_INCS = incs

SRCS = $(addprefix $(D_SRCS)/, \
			 main.c \
			 events.c \
			 window.c \
			 keyboard.c \
			 )

OBJS = $(SRCS:$(D_SRCS)/%.c=$(D_OBJS)/%.o)

INC = -I $(D_INCS)
INC += -I /usr/include/xcb

CFLAGS = -W -Wall -Werror -Wextra -pedantic
CFLAGS += $(INC)

LDFLAGS = `pkg-config --libs xcb xcb-util xcb-keysyms xcb-xkb xkbcommon xkbcommon-x11`

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)
	@echo -e "\033[38;5;10m=== $(NAME): Compiled !\033[0m"

$(D_OBJS)/%.o: $(D_SRCS)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find -name "*~" -delete -o -name "#*#" -delete
	$(RM) $(D_OBJS)
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: re
	Xephyr -br -ac -noreset -screen "1024x640" :1 &
	DISPLAY=:1 ./$(NAME)

dbg: CFLAGS += -g3 -O0 -DDEBUG
dbg: re
	@echo -e "\033[1;49;36m=== $(NAME): Debug mode\033[0m"

postnazi: CC := clang -Weverything
postnazi: re

nazi:
	@echo -e "\033[1;49;41m=== Attention: Nazi mode activated\033[0m"⏎
	$(MAKE) postnazi

help:
	@echo "Available rules : $(NAME) $(RULES)"

.PHONY: $(RULES)
