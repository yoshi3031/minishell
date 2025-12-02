/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:39:36 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/02 10:32:03 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_echo(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	(void)shell;
	if (cmd->str[i] && !ft_strcmp(cmd->str[i], "-n"))
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
