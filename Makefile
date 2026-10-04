CC = cc
CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

TARGET = codexion

FILES = main.c

SRC_DIR = src/
INCLUDE_DIR = include/

OBJ = $(addprefix $(),foo bar)


%.c: %.0
	$(CC) $(CFLAGS) $? -o $@







