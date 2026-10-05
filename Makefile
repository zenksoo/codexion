GREEN_SEP   = @printf "\033[42m \033[49m  "
RED_SEP     = @printf "\033[41m \033[49m  "


CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread

RM          = rm -rf

NAME        = codexion

FILES       = main.c
BUILD_DIR   = build/

SRC_DIR     = src/

INCLUDE_DIR = include/


OBJ = $(addprefix $(BUILD_DIR), $(FILES:.c=.o))


$(NAME): $(BUILD_DIR) $(OBJ)
	@printf "\033[33m"
	$(CC) $(CFLAGS) $(OBJ) -o $@
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
	@./$(NAME)
