NAME = cub3D

CC = clang

CFLAGS = -Wall -Wextra -Werror

SRCS = main.c\
       gc/gc.c\
       get_next_line/get_next_line_bonus.c\
       get_next_line/get_next_line_utils_bonus.c\
       map_parsing/checks.c\
       map_parsing/parse.c\
       map_parsing/utils.c

OBJDIR = objs
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SRCS)))

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

vpath %.c . gc get_next_line map_parsing

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
