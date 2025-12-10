/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:12:44 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 15:20:31 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include <stdlib.h>

typedef enum s_tokens
{
	PIPE = 1,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_IN,
	HEREDOC,
	WORD,
	DQUOTE_WORD,
	SQUOTE_WORD,
	ENV_VAR,
	EXIT_STATUS,
	END_OF_INPUT
}					t_tokens;

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				i;
	int				heredoc_fd;
	int				join_next;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}					t_lexer;

t_lexer				*lexer(const char *input);
t_lexer				*lexer_error(const char *msg);
void				append_token(t_lexer **list, t_lexer *new);
t_lexer				*new_token(char *str, t_tokens type);
void				read_dollar(const char **input, t_lexer **tokens);
void				read_operator(const char **input, t_lexer **tokens);
int					read_quoted(const char **input, t_lexer **tokens);
void				read_word(const char **input, t_lexer **tokens);
int					is_separator(char c);
int					is_quote(char c);
int					is_operator_start(char c);
int					is_word_char(char c);
void				skip_spaces(const char **input);
char				*ft_strndup(const char *s, size_t n);
int					read_one_char_op(const char **input, t_lexer **tokens);
int					read_two_char_op(const char **input, t_lexer **tokens);

#endif
