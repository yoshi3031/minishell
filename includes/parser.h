/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:56:10 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/08 07:38:40 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include "minishell.h"
# include <stdbool.h>
# include <stddef.h>


typedef struct s_parser_shell
{
	t_lexer *lexer_list;
	t_lexer *redirections;
	int num_redirections;
	struct s_shell *shell;
} t_parser_shell;

typedef struct s_shell
{
	char *args;
	char **paths;
	char **env;
	int pipes;
	int *pid;
	t_lexer *lexer_list;
	struct s_cmd *cmd;
	bool reset;
	bool heredoc;
	int error_num;
} t_shell;


typedef struct s_cmd
{
	char **str;
	int (*builtin)(t_shell *, struct s_cmd *);
	int num_redirections;
	t_lexer *redirections;
	struct s_cmd *next;
	struct s_cmd *prev;
} t_cmd;

int parser(t_shell *shell);

// cmd
t_cmd *init_cmd(t_parser_shell *parser_shell);

// cmd_utils
int	is_joinable(t_tokens token);
void free_cmd(t_cmd **lst);
void	add_cmd_back(t_cmd **lst, t_cmd *new_cmd);

// parser_utils
void				count_pipes(t_lexer *lexer_list, t_shell *shell);
t_parser_shell		init_parser_shell(t_lexer *lexer_list, t_shell *shell);

// parser_error
int					parser_double_token_error(t_tokens token);
int					pipe_errors(t_shell *shell, t_tokens token);
int					ft_error(int error);
void				free_arr(char **arr);

// node
void free_lexer(t_lexer **lst);
void remove_node(t_lexer **lst, int target_i);
t_lexer *create_node(char *str, int token);
void add_node_back(t_lexer **lst, t_lexer *new_node);

// libft
void ft_putstr_fd(char *s, int fd);
void *ft_calloc(size_t nmemb, size_t size);
char *ft_strdup(const char *s);
size_t ft_strlen(const char *s);

// shell
int					reset_shell(t_shell *shell);
void				free_arr(char **arr);
void				init_shell(t_shell *shell);
int					loop(t_shell *shell);

// redirections
int					remove_redirections(t_parser_shell *parser_shell);

// quotes
int					validate_quotes(char *args);

// main
void				print_parsed_commands(t_shell *shell);
#endif
