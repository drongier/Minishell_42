NAME	=	minishell
CC		=	cc
RL		=	-lreadline
CFLAGS	=	#-g -fsanitize=address #-Wall -Wextra -Werror
RM		=	rm -f -r

SRCC	=	minishell.c expander.c utils.c parser.c printers.c executer.c \
			environment.c bin/echo.c bin/pwd.c bin/cd.c bin/env.c bin/export.c \
			bin/unset.c bin/exit.c pathfinder.c pipe.c free.c  error.c \
			helper.c signals.c heredoc.c \
			lexer/lexer.c lexer/lexer_utils.c lexer/lexer_utils2.c
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
