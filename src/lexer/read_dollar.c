/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_dollar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:16:53 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:03:38 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ドル記号($)で始まるトークンを読み取る
// - `$?` は EXIT_STATUS トークンとして処理
// - `$VAR` 形式の環境変数は ENV_VAR トークンとして処理
// - それ以外の`$`は WORD トークンとして処理
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
void	read_dollar(const char **input, t_lexer **tokens)
{
	const char	*start;
	char		*var;

	// ドル記号の開始位置を記録
	start = *input;
	// 入力ポインタを '$' の次に進める
	(*input)++;
	// '$' の次が '?' の場合 (例: $?)
	if (**input == '?')
	{
		// ポインタを '?' の次に進め、EXIT_STATUSトークンを追加
		(*input)++;
		append_token(tokens, new_token("$?", EXIT_STATUS));
		return ;
	}
	// '$' の後の文字が英数字またはアンダースコアである限りポインタを進める
	// (環境変数名として有効な文字)
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	// ポインタが '$' の直後から動いていない場合 (例: "$ " や "$<")
	// '$' 単体をWORDトークンとして扱う
	if (*input == start + 1)
	{
		append_token(tokens, new_token("$", WORD));
		return ;
	}
	// '$' から変数名の終わりまでを文字列として複製
	var = ft_strndup(start, *input - start);
	// 複製した文字列でENV_VARトークンを作成し、リストに追加
	append_token(tokens, new_token(var, ENV_VAR));
	free(var);
}
