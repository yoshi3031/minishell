/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:02:15 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 11:08:05 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (!is_numeric(cmd->str[1]))
	{
		exit_numeric_error(cmd->str[1]);
		exit(255);
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
