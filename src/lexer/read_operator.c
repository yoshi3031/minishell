/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:35:34 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/02 10:32:40 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// シェル演算子（|、<、>、<<、>>）を読み取り、トークン化する
// 2文字の演算子を先に試してから、1文字の演算子を試す
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
void	read_operator(const char **input, t_lexer **tokens)
{
	if (read_two_char_op(input, tokens))
		return ;
	if (read_one_char_op(input, tokens))
		return ;
	lexer_error("unexpected operator");
}

// 2文字の演算子（<<, >>）を読み取る
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
// @return: 2文字演算子を読み取った場合は1、そうでない場合は0
int	read_two_char_op(const char **input, t_lexer **tokens)
{
	t_tokens	type;

	if (**input == '<' && *(*input + 1) == '<')
		type = HEREDOC;
	else if (**input == '>' && *(*input + 1) == '>')
		type = REDIR_APPEND;
	else
		return (0);
	*input += 2;
	append_token(tokens, new_token(NULL, type));
	return (1);
}

// 1文字の演算子（|、<、>）を読み取る
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
// @return: 1文字演算子を読み取った場合は1、そうでない場合は0
int	read_one_char_op(const char **input, t_lexer **tokens)
{
	t_tokens	type;

	if (**input == '|')
		type = PIPE;
	else if (**input == '<')
		type = REDIR_IN;
	else if (**input == '>')
		type = REDIR_OUT;
	else
		return (0);
	(*input)++;
	append_token(tokens, new_token(NULL, type));
	return (1);
}
