/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:02:15 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 15:00:05 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

// 文字列がlong long型の範囲を超えるかどうかをチェックする
// @param str: チェックする文字列
// @return: オーバーフローする場合は1、しない場合は0
int	is_overflow(const char *str)
{
	long long	res;
	int			sign;

	res = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && ft_isdigit(*str))
	{
		if (sign == 1 && (res > LONG_MAX / 10 || (res == LONG_MAX / 10 && (*str
						- '0') > LONG_MAX % 10)))
			return (1);
		if (sign == -1 && (res > -(LONG_MIN / 10) || (res == -(LONG_MIN / 10)
					&& (*str - '0') > -(LONG_MIN % 10))))
			return (1);
		res = res * 10 + (*str - '0');
		str++;
	}
	return (0);
}

// 文字列が数値として有効かどうかをチェックする
// 先頭と末尾の空白は許容する
// @param str: チェックする文字列
// @return: 数値として有効な場合は1、そうでない場合は0
int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

// メモリを解放し、指定された終了コードでシェルを終了する
// @param shell: シェルの状態を保持する構造体
// @param exit_code: 終了コード
void	clean_exit(t_shell *shell, int exit_code)
{
	if (shell->env)
		free_arr(shell->env);
	if (shell->paths)
		free_arr(shell->paths);
	if (shell->args)
		free(shell->args);
	if (shell->cmd)
		free_cmd(&shell->cmd);
	if (shell->pid)
		free(shell->pid);
	exit(exit_code);
}

// "numeric argument required"エラーメッセージを出力する
// @param arg: エラーの原因となった引数
void	exit_numeric_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

// "too many arguments"エラーメッセージを出力し、エラー番号を設定する
// @param shell: シェルの状態を保持する構造体
void	exit_too_many_args(t_shell *shell)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	shell->error_num = 1;
}
