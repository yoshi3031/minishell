/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_quoted.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:53:20 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:03:37 by yotakagi         ###   ########.fr       */
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

	// クォートされた文字列の開始位置を記録
	start = *input;
	// 開始クォート文字（' または "）を保存
	quote = **input;
	// 入力ポインタを開始クォートの次に進める
	(*input)++;
	// 文字列の終端か、対応する終了クォートが見つかるまでポインタを進める
	while (**input && **input != quote)
		(*input)++;
	// ループが終了したとき、現在の文字が終了クォートでなければ、クォートが閉じられていない
	if (**input != quote)
	{
		lexer_error("unclosed quote");
		return (0);
	}
	// 入力ポインタを終了クォートの次に進める
	(*input)++;
	// 開始クォートから終了クォートまでを含む部分文字列を複製
	quoted = ft_strndup(start, *input - start);
	// 開始クォートの種類に応じてトークンタイプを決定
	if (quote == '\'')
		type = SQUOTE_WORD;
	else
		type = DQUOTE_WORD;
	// 新しいトークンを作成してリストに追加
	append_token(tokens, new_token(quoted, type));
	free(quoted);
	return (1);
}
