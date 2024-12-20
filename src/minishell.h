/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chbachir <chbachir@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 15:02:35 by emaydogd          #+#    #+#             */
/*   Updated: 2024/11/26 14:43:35 by chbachir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _POSIX_C_SOURCE 200809L
# define NULL ((void*)0)
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include "../libft/libft.h"

# define BANNER "\
######################################################################################\n\
██╗  ██╗██████╗ ███████╗██╗  ██╗███████╗██╗     ██╗ \n\
██║  ██║╚════██╗██╔════╝██║  ██║██╔════╝██║     ██║ \n\
███████║ █████╔╝███████╗███████║█████╗  ██║     ██║ \n\
╚════██║██╔═══╝ ╚════██║██╔══██║██╔══╝  ██║     ██║ \n\
     ██║███████╗███████║██║  ██║███████╗███████╗███████╗ \n\
     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝ \n\
 \n\
 ██████╗██╗  ██╗ █████╗ ██╗  ██╗██╗██████╗       ███████╗███╗   ███╗██████╗ ███████╗ \n\
██╔════╝██║  ██║██╔══██╗██║ ██╔╝██║██╔══██╗      ██╔════╝████╗ ████║██╔══██╗██╔════╝ \n\
██║     ███████║███████║█████╔╝ ██║██████╔╝█████╗█████╗  ██╔████╔██║██████╔╝█████╗ \n\
██║     ██╔══██║██╔══██║██╔═██╗ ██║██╔══██╗╚════╝██╔══╝  ██║╚██╔╝██║██╔══██╗██╔══╝ \n\
╚██████╗██║  ██║██║  ██║██║  ██╗██║██████╔╝      ███████╗██║ ╚═╝ ██║██║  ██║███████╗ \n\
 ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═════╝       ╚══════╝╚═╝     ╚═╝╚═╝  ╚═╝╚══════╝ \n\
######################################################################################\n\
"

extern volatile sig_atomic_t g_signal;

typedef enum	s_token_type
{
	TOKEN_ARG = 0,
	TOKEN_PIPE,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_IN,
	TOKEN_COMMAND
}	t_token_type;

typedef struct s_lexer
{
	char			*input;
	size_t			pos;
	int				type;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_pipe 
{
    int read_fd;
    int write_fd;
} t_pipex;

typedef struct s_parser
{
	t_list			*args;
	int				infile;
	int				outfile;
	t_pipex 		pipex;
	struct s_parser	*next;
}	t_parser;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	char			*cmdline;
	char			*tmp_output;
	int				exit_status;
	int				flag_pipe;
	int				in_heredoc;
	struct s_lexer	*lexer;
	struct s_parser	*parser;
	struct s_env	*env;
}	t_shell;

/* LEXER */

void	lexer(t_shell *shell);
char *add_spaces_around_redirection(const char *cmdline);
void	free_split_res(char **split_res);
//static int	push(t_lexer **lexer, char *input, t_token_type type, size_t pos);

void	expander(t_shell *shell);
void	parser(t_shell *shell);

void	init_env(t_shell *shell, char **env);
int		env_push(t_env **env, char* key, char *value);
int		env_pop(t_env **env, char *key);

/* BUILTINS */
int		is_bin(char *cmd);
void	exec_bin(t_shell *shell, char *cmd, t_list *args);
void	exec_echo(t_shell *parser, t_list *args);
void	exec_cd(t_shell *shell, t_list *args);
void	exec_pwd(t_shell *shell);
void	exec_env(t_shell shell);
void	exec_export(t_shell *shell);
void	exec_unset(t_shell *shell);
void	exec_exit(t_shell *shell);
void	exec_start(t_shell *shell);
void	exec_cmd(char *path, t_list *args, t_shell *shell);

/* todo: DELETE */
void	print_lexer(t_shell shell);
void	print_cmdtable(t_shell shell);

int		correct_single_quotes(char * input);
void	write_single_quotes(char *input);

/* HELPER FUNCTIONS */
char	*trim(char *str);
void	ft_error(t_shell *shell, char *err_msg, char *args, int exit_status);
char 	**convert_env_to_array(t_env *env);
char 	*remove_quotes(char *str);
char	**list_to_array(t_list *args);

// Cleaning
void	cleanup(t_shell *shell);
void	free_lexer(t_lexer *lexer);
void	free_parser(t_parser *parser);
void	free_env(t_env *env);
void	free_envp(char **envp, int idx);

int		*piping();
char 	*get_external_cmd_path(t_shell *shell, char *cmd);
char	*ft_getenv(t_shell *shell, char *key);

// Executer
int		dollar_at_beginning(char *cmdline);
int		ft_isspace(char c);
void 	exec_with_pipe(t_shell *shell);

//error check

void		check_error_token_redi(t_shell *shell);
int		ft_env_size(t_env *env);

// Signals

void	handle_sigint(int sig);
void	setup_signal_handlers(void);

// heredoc
void	handle_heredoc(t_shell *shell, t_parser *parser, const char *delimiter);

// signals
void use_signals(void);

#endif
