NAME =		minishell
TEST =		minishell_test

SRC =		main.c sig_handler.c split_quotes.c trim_quotes.c split_command.c
SRC_TEST =	main_test.c split_quotes.c trim_quotes.c split_command.c  final_split.c ft_gest_ambiental.c ft_check_syntax.c \
			ft_create_cmds.c ft_create_node.c ft_quit.c ft_heredoc.c ft_exe.c ft_signals.c

UTILS =		utils1.c utils2.c utils1_free.c utils_print.c 

LIB =		./libft/libft.a
GNL =		./gnl/get_next_line_utils_bonus.c ./gnl/get_next_line_bonus.c

CFLAGS =	-Wall -Werror -Wextra -g

all: $(NAME)

$(NAME): $(SRC) $(UTILS)
	@make -s -C libft
	@gcc $(SRC) $(UTILS) $(CFLAGS) $(LIB) -o $(NAME) -lreadline

test: $(SRC_TEST) $(UTILS)
	@make bonus -s -C libft
	@gcc $(SRC_TEST) $(UTILS) $(GNL) $(CFLAGS) $(LIB) -o $(TEST) -lreadline

clean:
	@make clean -s -C libft

fclean:
	@make fclean -s -C libft
	@rm -f $(NAME)
	@rm -f $(TEST)

re: fclean all
