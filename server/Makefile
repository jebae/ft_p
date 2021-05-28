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
	run_server.c\
	utils.c\

SRC_LS = payload.c\
	file_list.c\
	network_io.c\

SRC_CD = network_io.c\

SRC_PWD = network_io.c\
	payload.c\

SRC_GET = network_io.c\

SRC_PUT = network_io.c\

SRC_DEL = network_io.c\

SRC_MKDIR = network_io.c\

SRC_RMDIR = network_io.c\
	remove_dir.c\

SRC_FILE_TRANSFER = chunk_buffer.c\
	sender.c\
	receiver.c\
	payload.c\
	file.c\
	utils.c\

# objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/ls_, $(SRC_LS:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/cd_, $(SRC_CD:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/pwd_, $(SRC_PWD:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/get_, $(SRC_GET:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/put_, $(SRC_PUT:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/del_, $(SRC_DEL:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/mkdir_, $(SRC_MKDIR:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/rmdir_, $(SRC_RMDIR:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/tranfer_, $(SRC_FILE_TRANSFER:.c=.o))

def = ''

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/ls_%.o: $(SRC_DIR)/ls/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/cd_%.o: $(SRC_DIR)/cd/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/pwd_%.o: $(SRC_DIR)/pwd/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/get_%.o: $(SRC_DIR)/get/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/put_%.o: $(SRC_DIR)/put/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/del_%.o: $(SRC_DIR)/del/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/mkdir_%.o: $(SRC_DIR)/mkdir/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/rmdir_%.o: $(SRC_DIR)/rmdir/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/tranfer_%.o: $(FILE_TRANSFTER)/srcs/%.c
	$(CC) $(def) $(CFLAGS) $(INCLUDES) -c $< -o $@

TEST = $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp

# command
all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(def) $(CFALGS) $(INCLUDES) $(LIBS) $(OBJS) -o $@
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
		$(filter-out ./srcs/main.c, $(addprefix $(FILE_TRANSFTER)/srcs/, $(SRC_FILE_TRANSFER)) $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/**/*.c) $(TEST))\
		-o $@

.PHONY: test all clean fclean re