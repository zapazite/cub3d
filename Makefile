NAME = cub3D

CC = clang

CFLAGS = -Wall -Wextra -Werror -g

SRCS = 	main.c\
		gc/gc.c\
		get_next_line/get_next_line_bonus.c\
		get_next_line/get_next_line_utils_bonus.c

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

clean:
	rm -rf ${OBJS}

fclean: clean
	rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean re%
