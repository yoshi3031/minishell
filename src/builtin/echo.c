/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:39:36 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:09:32 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 文字列が-nフラグ（または-nnn...のような連続したn）であるかを確認する
// @param str: チェックする文字列
// @return: -nフラグである場合は1、そうでない場合は0
static int	is_n_flag(const char *str)
{
	int	i;

	// 文字列がNULLか、1文字目が'-'でなければフラグではない
	if (!str || str[0] != '-')
		return (0);
	i = 1;
	// "-n"のように、'-'の後に少なくとも1文字必要
	if (str[i] == '\0')
		return (0);
	// '-'に続く文字がすべて'n'であるかチェック
	while (str[i])
	{
		// 'n'以外の文字があればフラグではない
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

	(void)shell;
	i = 1;
	// nl (newline) フラグ。1なら改行あり、0なら改行なし
	nl = 1;
	// -n, -nn, -nnn... といったフラグが続く限りループ
	while (cmd->str[i] && is_n_flag(cmd->str[i]))
	{
		// -nフラグが見つかったので、末尾の改行は出力しない
		nl = 0;
		i++;
	}
	// 残りの引数を出力
	while (cmd->str[i])
	{
		// 引数の文字列を出力
		ft_putstr_fd(cmd->str[i], 1);
		// 次の引数が存在すれば、スペースを出力
		if (cmd->str[i + 1])
			write(1, " ", 1);
		i++;
	}
	// nlフラグが1（-nフラグがなかった）の場合、最後に改行を出力
	if (nl)
		write(1, "\n", 1);
	return (0);
}
