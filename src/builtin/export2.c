/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:28:38 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:09:34 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 2次元配列のメモリを解放する
// @param arr: 解放する2次元配列
void	free_2d_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	// 配列の各要素（文字列）を解放
	while (arr[i])
		free(arr[i++]);
	// 配列自体を解放
	free(arr);
}

// 2次元配列の要素数を数える
// @param arr: 数える2次元配列
// @return: 配列の要素数
int	count_2d_arr(char **arr)
{
	int	i;

	i = 0;
	// 配列がNULLでない限り、NULL終端まで要素を数える
	while (arr && arr[i])
		i++;
	return (i);
}

// "not a valid identifier"エラーメッセージをexport用に表示する
// @param s: エラーの原因となった識別子
void	export_error(const char *s)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)s, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

// `KEY=value`形式の文字列からキー部分を抽出する
// @param arg: `KEY=value`形式の文字列
// @return: 抽出されたキー文字列
char	*get_key(const char *arg)
{
	size_t	len;

	// 文字列の先頭から'='が現れるまでの長さを計算
	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	return (ft_substr(arg, 0, len));
}

// `KEY=value`形式の文字列から値部分を抽出する
// @param arg: `KEY=value`形式の文字列
// @return: 抽出された値文字列。'='がない場合はNULL
char	*get_val(const char *arg)
{
	char	*val;

	// 文字列内で最初の'='を探す
	val = ft_strchr(arg, '=');
	if (!val)
		return (NULL);
	// '='の次の文字から末尾までを複製して返す
	return (ft_strdup(val + 1));
}
