/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:39:36 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/10 13:35:45 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
