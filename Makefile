GREEN_SEP   = @printf "\033[42m \033[49m  "
RED_SEP     = @printf "\033[41m \033[49m  "


CC          = cc
CFLAGS      = -g -Wall -Wextra -Werror -pthread
MEM_PROB	= --fsanitize=address -gdb

RM          = rm -rf

NAME        = codexion

FILES       = main.c\
              commons.c\
			  cli_argparser.c\
			  simulation.c\
			  init.c\
			  coder.c\
			  monitor.c


BUILD_DIR   = build/

SRC_DIR     = src/

INCLUDE_DIR = include/


OBJ = $(addprefix $(BUILD_DIR), $(FILES:.c=.o))


$(NAME): $(BUILD_DIR) $(OBJ)
	@printf "\033[33m"
	$(CC) $(CFLAGS) -fsanitize=address -g3 $(OBJ) -o $@
	@printf "\033[39m"


$(BUILD_DIR):
	$(GREEN_SEP)
	mkdir $(BUILD_DIR)


$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	@printf "\033[32m"
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
	@printf "\033[39m"


all: $(NAME)


clean:
	$(RED_SEP)
	$(RM) $(OBJ) $(BUILD_DIR)


fclean: clean
	$(RED_SEP)
	@printf "\033[31m"
	$(RM) $(NAME)
	@printf "\033[39m"


re: fclean all


run: $(NAME)
	@./$(NAME) 4 400 200 200 200 2 200 fifo
