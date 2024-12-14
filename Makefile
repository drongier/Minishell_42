NAME	=	minishell
CC		=	cc
RL		=	-lreadline
CFLAGS	=	-g -Wall -Wextra -Werror -fsanitize=address
RM		=	rm -f -r

SRCC	=	minishell.c \
			lexer/lexer.c lexer/lexer_utils_1.c lexer/lexer_utils_2.c lexer/lexer_utils_3.c lexer/lexer_utils_4.c\
			expander/expander.c expander/expander_utils_1.c expander/expander_utils_2.c \
			parser/parser.c parser/parser_utils_1.c parser/parser_utils_2.c \
			executer/executer.c executer/pathfinder.c executer/exec_utils_1.c executer/exec_utils_2.c executer/exec_utils_3.c \
			env/environment.c bin/echo.c bin/pwd.c bin/cd.c bin/env.c bin/export.c \
			bin/unset.c bin/exit.c pipes/pipe.c free.c  error.c \
			helper.c signals.c heredoc/heredoc.c utils/utils_2.c \
			utils/mini_utils.c pipes/pipe_utils.c heredoc/heredoc_utils.c utils/utils.c
OBJD	=	obj
OBJDBIN	=	obj/bin
SRCD	=	src
SRCS	=	$(addprefix $(SRCD)/,$(SRCC))
OBJS	=	$(addprefix $(OBJD)/,$(SRCC:.c=.o))
LIBFT	=	libft/libft.a

all:		$(NAME)

$(LIBFT):
	@make -C libft

$(NAME):	$(LIBFT) $(OBJS)
			@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(RL)
			@echo "Compile Minishell"

$(OBJD)/%.o: $(SRCD)/%.c
			@mkdir -p $(dir $@)
			@$(CC) $(CFLAGS) -c $< -o $@

clean:
			@$(RM) $(OBJD)
			@make clean -C libft
			@echo "Remove object files"

fclean:		clean
			@$(RM) $(NAME)
			@make fclean -C libft
			@echo "Remove program"

re:			fclean all

.PHONY:		all clean fclean re
