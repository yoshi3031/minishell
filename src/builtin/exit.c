/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 15:27:01 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 14:51:08 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	minishell_exit(t_shell *shell, t_cmd *cmd)
{
	int	exit_code;

	if (!cmd->str[1])
		clean_exit(shell, shell->error_num);
	if (!is_numeric(cmd->str[1]) || is_overflow(cmd->str[1]))
	{
		exit_numeric_error(cmd->str[1]);
		clean_exit(shell, 2);
	}
	if (cmd->str[2])
	{
		exit_too_many_args(shell);
		return (1);
	}
	exit_code = ft_atoi(cmd->str[1]);
	clean_exit(shell, (unsigned char)exit_code);
	return (0);
}
