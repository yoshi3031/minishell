/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:39:36 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 15:58:16 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 文字列が-nフラグ（または-nnn...のような連続したn）であるかを確認する
// @param str: チェックする文字列
// @return: -nフラグである場合は1、そうでない場合は0
static int	is_n_flag(const char *str)
{
	int	i;

	i = 1;
	if (!str || str[0] != '-')
		return (0);
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

// echoコマンドを実行する
// -nフラグを処理し、引数をスペースで区切って出力する
// @param shell: シェルの状態を保持する構造体（この関数では未使用）
// @param cmd: コマンドの情報を保持する構造体
// @return: 常に0
int	minishell_echo(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	(void)shell;
	while (cmd->str[i] && is_n_flag(cmd->str[i]))
	{
		nl = 0;
		i++;
	}
	while (cmd->str[i])
	{
		ft_putstr_fd(cmd->str[i], 1);
		if (cmd->str[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (nl)
		write(1, "\n", 1);
	return (0);
}
