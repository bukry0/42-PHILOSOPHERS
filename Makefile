NAME = philo

CC = clang

CFLAGS = -Wall -Werror -Wextra -pthread -g -fsanitize=thread

SRC =	philo.c \
		init.c \
		monitor.c \
		mutex_functions.c \
		routine.c \
		utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all, clean, fclean, re