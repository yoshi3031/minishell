/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:19:47 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 12:14:59 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 文字がクォート（シングルまたはダブル）であるかを確認する
int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

// 文字が演算子の開始文字であるかを確認する
int	is_operator_start(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

// 文字が単語を構成する文字であるかを確認する
int	is_word_char(char c)
{
	return (!is_separator(c));
}

// 空白文字（スペース、タブ）をスキップする
void	skip_spaces(const char **input)
{
	while (**input == ' ' || **input == '\t')
		(*input)++;
}

// レクサーのエラーメッセージを標準エラー出力に表示する
t_lexer	*lexer_error(const char *msg)
{
	write(2, "lexer error: ", 14);
	while (*msg)
		write(2, msg++, 1);
	write(2, "\n", 1);
	return (NULL);
}
