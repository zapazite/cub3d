NAME = cub3D

CC = clang

LIBS = -L./minilibx-linux -lXext -lXfixes -lX11 -lm -lmlx_Linux

CFLAGS = -Wall -Wextra -Werror -o3 -g

BONUS = 0

SRCS = main.c\
       gc/gc.c\
       get_next_line/get_next_line_bonus.c\
       get_next_line/get_next_line_utils_bonus.c\
       map_parsing/checks.c\
       map_parsing/parse.c\
       map_parsing/utils.c\
       map_parsing/utils2.c\
       render.c\
       player_move.c\
       draw.c\
       draw2.c\
       draw3.c\
       raycast.c\
       raycast_utils.c\
       render_utils.c\
       render_utils2.c\
       render_utils3.c\
       render_utils4.c\
       world_manager_bonus.c

OBJDIR = objs
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SRCS)))

all: $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) -DBONUS=$(BONUS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -DBONUS=$(BONUS) -c $< -o $@

vpath %.c . gc get_next_line map_parsing

bonus:
	$(MAKE) BONUS=1 all

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
