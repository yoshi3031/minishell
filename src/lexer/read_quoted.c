/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_quoted.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:53:20 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/05 12:59:05 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// クォートされた文字列（シングルまたはダブル）を読み取り、トークン化する
// 1. 開始クォートを特定
// 2. 対応する終了クォートまで読み進める
// 3. 閉じられていない場合はエラー
// 4. クォートを含む文字列全体を SQUOTE_WORD または DQUOTE_WORD としてトークン化する
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
// @return: 成功した場合は1、閉じられていないクォートがある場合は0
int	read_quoted(const char **input, t_lexer **tokens)
{
	char		quote;
	const char	*start;
	char		*quoted;
	t_tokens	type;

	start = *input;
	quote = **input;
	(*input)++;
	while (**input && **input != quote)
		(*input)++;
	if (**input != quote)
	{
		lexer_error("unclosed quote");
		return (0);
	}
	(*input)++;
	quoted = ft_strndup(start, *input - start);
	if (quote == '\'')
		type = SQUOTE_WORD;
	else
		type = DQUOTE_WORD;
	append_token(tokens, new_token(quoted, type));
	free(quoted);
	return (1);
}
