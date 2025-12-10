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
