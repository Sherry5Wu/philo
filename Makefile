NAME = philo
CC := cc
FLAG := -Wall -Wextra -Werror
HEADER = ./include/philo.h

# SANITIZER = -fsanitize=thread

SRC = init.c parsing.c philo.c utils.cc
FULL_SRC = $(addprefix ./src/, $(SRC))

all: $(NAME)

$(NAME): $(FULL_SRC) $(HEADER)
	@$(CC) $(FLAG)  $(FULL_SRC) -o $<

clean:
	@rm -f $(NAME)

fclean: clean
	@rm -f $(NAME)

re: fclean all


.PHONY: all clean fclean re