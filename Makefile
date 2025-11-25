NAME		=	minishell
BONUS_NAME	= 	_bonus

HEADER	=	minishell.h

CC		=	cc
CFLAGS	=	-Wall -Wextra -Werror -O2 -g -fsanitize=address -fno-omit-frame-pointer

# Libft directory
LIBFT_DIR	=	./libft
LIBFT		=	$(LIBFT_DIR)/libft.a
LIBS		=	-lreadline -lncurses

SRC_DIR			=	src
OBJ_DIR			=	src_o
SRC_BONUS_DIR	=	src_bonus
OBJ_BONUS_DIR	=	src_bonus_o

SRC			=	src/basic_linked.c src/cd.c src/classify.c src/echo.c src/env.c \
src/error_exit.c src/execution_utils.c src/execution.c src/exit.c src/expansion_utils.c \
src/expansion.c src/export.c src/garbage_colector.c src/gc_wrappers.c src/heredoc.c \
src/init.c src/lexer_helpers.c src/lexer.c src/main.c src/pipe.c src/pwd.c src/redirections.c \
src/signals.c src/syntax_errors.c src/token_linked.c src/unset.c src/utils.c \

OBJ			=	$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

# COLORS
BLUE	= 	\033[38;5;27m
CYAN	= 	\033[38;5;51m
PINK	=	 \033[38;5;211m
GREEN	= 	\033[38;5;72m
TURQ	= 	\033[38;5;37m
WHITE	= 	\033[38;5;15m
YELLOW	= 	\033[38;5;220m
C		=	\033[38;5;
O		=	37
L		=	m
SHIFT	=	$(eval O=$(shell echo $$((($(O)%13)+1))))

LBOR	=	"▌│█║▌║▌║ "
RBOR	= 	" ║▌║▌║█│▌"

all: $(NAME)
	@echo "$(CYAN)"
	@echo "\t⠀⠀⠀⠀⠀⣀⣤⡴⢀⣠⣤⣴⣶⡶⠿⠿⠿⠿⣿⣿⣶⣶⣦⣟⡶⢦⣄⡀⠀⠀⠀"
	@echo "\t⠀⠀⢀⣴⡿⠛⣡⡾⠛⢋⣩⡄⠀⠀⠀⠀⠀⠀⠀⠀⠶⣭⣙⠻⢿⣷⣌⢙⢷⣄⠀"
	@echo "\t⠀⣰⣿⠏⠌⣾⠋⢀⣴⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢳⡄⠙⣿⡆⠀⢻⡆"
	@echo "\t⠀⢿⣿⡄⠀⢿⡄⠘⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡼⡣⣠⡿⠃⠀⣼⡇"
	@echo "\t⠀⠘⢿⣿⣦⣄⠙⠷⣬⣛⠷⣦⣤⣤⣀⣀⣀⣀⣤⣤⡴⠞⣫⣵⠾⠋⣁⣤⣾⠏⠀"
	@echo "\t⠀⠀⠀⠙⠿⣿⣿⣷⣶⣭⣍⣐⣀⣉⠉⠉⠉⠉⢁⣀⣀⣀⣤⣴⣶⡿⣿⡏⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⢸⣿⡟⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⠿⠿⠿⠛⠛⠩⣁⣴⡿⠃⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠈⢿⣿⣮⣟⠿⣿⣿⣷⣶⣶⣶⣶⣶⣶⣶⣶⣾⠿⠟⠋⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⢀⣾⠟⢿⣿⣿⣶⣭⣉⡉⠉⠉⠉⠩⢍⣩⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠈⢿⣧⣄⠈⠉⠛⠻⠿⠿⠿⣿⣿⢿⡿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠀⣴⡿⠿⣿⣶⣤⣄⣀⣀⣀⣀⣀⣼⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠀⠙⢿⣶⣦⣍⣉⡛⠛⠛⠛⠛⣻⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠀⠀⠸⣿⣽⣛⡛⠿⠿⠿⠿⣿⡋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣿⣿⠿⠿⠿⣯⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣟⣛⣛⣻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠹⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "\t⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀"
	@echo "Made By Turmoil!\t  ⠙⡇"
	@echo "$(WHITE)"

$(NAME): $(OBJ) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L$(LIBFT_DIR) -lft $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

%.o: %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

submodule:
	@echo "$(CYAN)$(LBOR) Initializing submodules... $(RBOR)$(WHITE)"
	@git submodule update --init --recursive
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "\n\t   $(PINK)$(LBOR)Cleaning$(RBOR)$(WHITE)\n"
	@rm -rf $(OBJ_DIR) $(OBJ_BONUS_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "\n\t   $(CYAN)$(LBOR)Clean af$(RBOR)$(WHITE)\n"
	@rm -rf $(NAME) $(BONUS_NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re:
	@echo "\n\t$(BLUE)$(LBOR)Awwwww shiiiiiit$(RBOR)$(WHITE)\n"
	@echo "\n\t$(BLUE)$(LBOR)Here we go again$(RBOR)$(WHITE)\n"
	@$(MAKE) fclean
	@$(MAKE) all

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

.PHONY: all clean fclean re bonus
