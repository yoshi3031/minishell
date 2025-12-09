/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:35:34 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:03:38 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// シェル演算子（|、<、>、<<、>>）を読み取り、トークン化する
// 2文字の演算子を先に試してから、1文字の演算子を試す
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
void	read_operator(const char **input, t_lexer **tokens)
{
	// まず2文字の演算子（<<, >>）として読み取りを試みる
	if (read_two_char_op(input, tokens))
		return ;
	// 2文字演算子でなければ、1文字の演算子（|, <, >）として読み取りを試みる
	if (read_one_char_op(input, tokens))
		return ;
	// どちらでもない場合は、予期せぬ演算子としてエラーを報告（このパスには通常到達しないはず）
	lexer_error("unexpected operator");
}

// 2文字の演算子（<<, >>）を読み取る
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
// @return: 2文字演算子を読み取った場合は1、そうでない場合は0
int	read_two_char_op(const char **input, t_lexer **tokens)
{
	t_tokens	type;

	// "<<" (ヒアドキュメント) の場合
	if (**input == '<' && *(*input + 1) == '<')
		type = HEREDOC;
	// ">>" (リダイレクト追記) の場合
	else if (**input == '>' && *(*input + 1) == '>')
		type = REDIR_APPEND;
	else
		// 2文字の演算子ではない
		return (0);
	// 2文字分ポインタを進め、対応するトークンを追加
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

	// "|" (パイプ) の場合
	if (**input == '|')
		type = PIPE;
	// "<" (リダイレクト入力) の場合
	else if (**input == '<')
		type = REDIR_IN;
	// ">" (リダイレクト出力) の場合
	else if (**input == '>')
		type = REDIR_OUT;
	else
		// 1文字の演算子ではない
		return (0);
	// 1文字分ポインタを進め、対応するトークンを追加
	(*input)++;
	append_token(tokens, new_token(NULL, type));
	return (1);
}
