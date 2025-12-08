/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_dollar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:16:53 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/08 14:41:26 by yotakagi         ###   ########.fr       */
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

	start = *input;
	(*input)++;
	if (**input == '?')
	{
		(*input)++;
		append_token(tokens, new_token("$?", EXIT_STATUS));
		return ;
	}
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	if (*input == start + 1)
	{
		append_token(tokens, new_token("$", WORD));
		return ;
	}
	var = ft_strndup(start, *input - start);
	append_token(tokens, new_token(var, ENV_VAR));
	free(var);
}
