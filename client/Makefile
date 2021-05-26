# name
NAME = client

# path
SRC_DIR = ./srcs
INC_DIR = ./includes
OBJ_DIR = ./objs
LIB_DIR = ../libs
LIBFT = $(LIB_DIR)/libft
FILE_TRANSFTER = ../file_transfer

# compiler option
CC = gcc
CFLAGS = -Werror -Wextra -Wall

INCLUDES = -I $(INC_DIR)\
	-I $(LIBFT)/includes\
	-I $(FILE_TRANSFTER)/includes\

LIBS = -L $(LIBFT) -lft

# srcs
SRCS = console.c\
	main.c\
	ls.c\
	cd.c\
	pwd.c\
	get.c\
	put.c\
	del.c\
	mkdir.c\
	rmdir.c\
	lls.c\
	utils.c\

SRC_FILE_TRANSFER = chunk_buffer.c\
	sender.c\
	receiver.c\
	payload.c\
	file.c\
	utils.c\

# objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/transfer_, $(SRC_FILE_TRANSFER:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/transfer_%.o: $(FILE_TRANSFTER)/srcs/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

TEST = $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp

# command
all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFALGS) $(INCLUDES) $(LIBS) $(OBJS) -o $@
	mkdir -p .storage

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

def = ''

test:
	$(MAKE) -C $(LIBFT)
	g++\
		-Wall -Wextra -std=c++11\
		$(def)\
		-DCHUNK_COUNT=10\
		-lgtest\
		$(LIBS)\
		$(INCLUDES)\
		$(filter-out ./srcs/main.c $(wildcard ./srcs/*.test.c) $(wildcard ./srcs/**/*.test.c), $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c) $(TEST))\
		-o $@

.PHONY: test all clean fclean re