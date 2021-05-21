# name
NAME = server

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
SRCS = cmd_route.c\
	main.c\
	utils.c\

SRC_LS = payload.c\
	file_list.c\
	network_io.c\

SRC_CD = network_io.c\

SRC_FILE_TRANSFER = chunk_buffer.c\
	sender.c\
	receiver.c\
	payload.c\
	utils.c\

# objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/ls_, $(SRC_LS:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/cd_, $(SRC_CD:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/tranfer_, $(SRC_FILE_TRANSFER:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/ls_%.o: $(SRC_DIR)/ls/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/cd_%.o: $(SRC_DIR)/cd/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/tranfer_%.o: $(FILE_TRANSFTER)/srcs/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

TEST = $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp

# command
all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFALGS) $(INCLUDES) $(LIBS) $(OBJS) -o $@

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
		$(filter-out ./srcs/main.c, $(addprefix $(FILE_TRANSFTER)/srcs/, $(SRC_FILE_TRANSFER)) $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c) $(TEST))\
		-o $@

.PHONY: test all clean fclean re