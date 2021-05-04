# name
NAME = libfiletransfer.a

# path
SRC_DIR = ./srcs
INC_DIR = ./includes
OBJ_DIR = ./objs
LIB_DIR = ../libs
LIBFT = $(LIB_DIR)/libft
FT_PRINTF = $(LIB_DIR)/ft_printf

# compiler
CC = gcc
CFLAGS = -Wall -Werror -Wextra
INCLUDES = -I $(INC_DIR)\
	-I $(LIBFT)/includes\
	-I $(FT_PRINTF)/includes

LIBS = -L $(LIBFT) -lft\
	-L $(FT_PRINTF) -lftprintf\

# srcs
SRCS = chunk_buffer.c

TEST = ./srcs/*.test.cpp

# objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# command
all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $@ $(OBJS)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

$(OBJ_DIR):
	mkdir -p $@

def = ''

test:
	$(MAKE) -C $(LIBFT)
	$(MAKE) -C $(FT_PRINTF)
	g++\
		-Wall -Wextra -std=c++11\
		$(def)\
		-DCHUNK_COUNT=10\
		-lgtest\
		$(LIBS)\
		$(INCLUDES)\
		srcs/*.c $(TEST)\
		-o $@

.PHONY: test
