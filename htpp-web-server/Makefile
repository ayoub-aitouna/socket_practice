
NAME = server
OBJ_DIR			:= .compiled
SRC				:= $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c)  $(wildcard src/**/**/**/*.c)
OBJ				:= $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
# CFLAGS			:= -fsanitize=address -g3
all: $(NAME)

$(OBJ_DIR)/%.o : %.c
	@mkdir -p $(dir $@)
	@echo "=> $@  ${GREEN}Created.${RESET} "
	@gcc $(CFLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	@gcc $(CFLAGS) $(OBJ) -o $(NAME)


clean:
	@rm -rf $(OBJ) $(OBJ_DIR) 

fclean: clean
	@rm -rf $(NAME)

re: clean fclean $(NAME)

.PHONY: clean fclean