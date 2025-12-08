/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:28:38 by yotakagi          #+#    #+#             */
/*   Updated: 2025/06/27 02:45:29 by yotakagi         ###   ########.fr       */
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
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

// 2次元配列の要素数を数える
// @param arr: 数える2次元配列
// @return: 配列の要素数
int	count_2d_arr(char **arr)
{
	int	i;

	i = 0;
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

	val = ft_strchr(arg, '=');
	if (!val)
		return (NULL);
	return (ft_strdup(val + 1));
}
