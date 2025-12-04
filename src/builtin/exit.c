/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:02:15 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 15:06:01 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

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
	while (*str)
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
int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	minishell_exit(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	ft_putstr_fd("exit\n", 2);
	if (!cmd->str[1])
		exit(shell->error_num);
	if (!is_numeric(cmd->str[1]) || is_overflow(cmd->str[1]))
	{
		exit_numeric_error(cmd->str[1]);
		exit(2);
	}
	if (cmd->str[2])
	{
		exit_too_many_args(shell);
		return (1);
	}
	exit_code = ft_atoi(cmd->str[1]);
	exit(exit_code);
}

void	exit_numeric_error(const char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	exit_too_many_args(t_shell *shell)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	shell->error_num = 1;
}
