NAME = philo
CC := cc
# -fsanitize=address,thread,undefined for testing only
CFLAG := -Wall -Wextra -Werror -g -pthread
HEADER = ./include

GREEN := \033[1;92m
DEFAULT := \033[0;39m

SRCS_DIR = ./src
SRC = init_philos.c init_table.c libft.c monitor.c philo.c routine_actions.c \
		routine.c stop_simulation.c utils.c
SRCS = $(addprefix $(SRCS_DIR)/, $(SRC))

OBJS_DIR = ./obj
OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(CC) $(CFLAG) $(OBJS) -o $@
	@echo "$(GREEN)Philo has been generated successfully!$(DEFAULT)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@$(CC) $(CFLAG) -I$(HEADER) -c $< -o $@

clean:
	@rm -rf $(OBJS_DIR)
	@echo "$(GREEN)Objects have been cleaned.$(DEFAULT)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(GREEN)Philo has been cleaned.$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re
