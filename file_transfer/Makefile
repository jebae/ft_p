# path
SRC_DIR = ./srcs
INC_DIR = ./includes
LIB_DIR = ../libs
LIBFT = $(LIB_DIR)/libft

# compiler
CC = gcc
CFLAGS = -Wall -Werror -Wextra
INCLUDES = -I $(INC_DIR)\
	-I $(LIBFT)/includes\

LIBS = -L $(LIBFT) -lft\

# srcs
SRCS = chunk_buffer.c\
	sender.c\
	receiver.c\
	utils.c\

TEST = ./srcs/*.test.cpp

# command
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
		$(filter-out ./srcs/*_main.c, $(addprefix ./srcs/, $(SRCS))) $(TEST)\
		-o $@
	$(CC) $(INCLUDES) $(LIBS) $(addprefix ./srcs/, $(SRCS)) ./srcs/sender_test_main.c -o sender
	$(CC) -DCHUNK_SIZE=200 -DCHUNK_COUNT=4 $(INCLUDES) $(LIBS) $(addprefix ./srcs/, $(SRCS)) ./srcs/receiver_test_main.c -o receiver
	$(CC) $(INCLUDES) $(LIBS) $(addprefix ./srcs/, $(SRCS)) ./srcs/interactive_receiver.c -o interactive_receiver
	$(CC) $(INCLUDES) $(LIBS) $(addprefix ./srcs/, $(SRCS)) ./srcs/asap_receiver.c -o asap_receiver
	$(CC) $(INCLUDES) $(LIBS) $(addprefix ./srcs/, $(SRCS)) ./srcs/interactive_sender.c -o interactive_sender

.PHONY: test
