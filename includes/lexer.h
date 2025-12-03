/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 14:12:44 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 13:59:02 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"
# include <stdlib.h>

typedef enum s_tokens
{
	PIPE = 1,
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	REDIR_IN,     // <
	HEREDOC,      // <<
	WORD,         // 通常の文字列
	DQUOTE_WORD,  // ダブルクォート付き文字列
	SQUOTE_WORD,  // シングルクォート付き文字列
	ENV_VAR,      // $変数
	EXIT_STATUS,  // $?
	END_OF_INPUT  // EOF
}		t_tokens;

typedef struct s_lexer
{
	char *str;      // トークンの文字列（例: "echo"）
	t_tokens token; // トークンの種類（例: WORD, PIPE など）
	int i;          // トークンのインデックス番号
	int	heredoc_fd;
	int	join_next;
	struct s_lexer *next; // 次のトークン（→）
	struct s_lexer *prev; // 前のトークン (←)
	int	join_next;
}		t_lexer;

t_lexer	*lexer(const char *input);
t_lexer	*lexer_error(const char *msg);
void	append_token(t_lexer **list, t_lexer *new); // TODO 後でparserと共通化
t_lexer	*new_token(char *str, t_tokens type);    // TODO 後でparserと共通化
void	read_dollar(const char **input, t_lexer **tokens);
void	read_operator(const char **input, t_lexer **tokens);
int		read_quoted(const char **input, t_lexer **tokens);
void	read_word(const char **input, t_lexer **tokens);
// utils
int		is_separator(char c);
int		is_quote(char c);
int		is_operator_start(char c);
int		is_word_char(char c);
void	skip_spaces(const char **input);
char	*ft_strndup(const char *s, size_t n);
int		read_one_char_op(const char **input, t_lexer **tokens);
int		read_two_char_op(const char **input, t_lexer **tokens);

#endif // !LEXER_H
