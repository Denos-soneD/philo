NAME = philo
CFLAGS = -Wall -Wextra -Werror -pthread -g -fsanitize=thread
CC = cc
BUILD_DIR = build
SRC_DIR = code/
SRC = $(addprefix $(SRC_DIR),main.c ft_atoi.c init.c print_msg.c routine.c ft_strncmp.c forks_mutex.c monitor.c utils.c)
HEADER = code/philo.h

OBJECTS = $(SRC:%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR) $(BUILD_DIR)/code

$(NAME): $(BUILD_DIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

$(BUILD_DIR)/%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re 