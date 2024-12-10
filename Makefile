NAME	=	minishell
CC		=	cc
RL		=	-lreadline
CFLAGS	=	-Wall -Wextra -Werror -g #-fsanitize=address
RM		=	rm -f -r

SRCC	=	minishell.c expander/expander.c expander/expander_utils_1.c expander/expander_utils_2.c utils.c parser/parser.c \
			parser/parser_utils_1.c parser/parser_utils_2.c executer/executer.c \
			executer/exec_utils.c environment.c bin/echo.c bin/pwd.c bin/cd.c bin/env.c bin/export.c \
			bin/unset.c bin/exit.c executer/pathfinder.c pipe.c free.c  error.c \
			helper.c signals.c heredoc.c executer/exec_utils2.c \
			lexer/lexer.c lexer/lexer_utils.c lexer/lexer_utils2.c mini_utils.c pipe_utils.c heredoc_utils.c
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
