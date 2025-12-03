/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:28:51 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/03 02:29:25 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor(t_shell *shell)
{
	expand_all_cmds(shell->cmd, shell->env, shell->error_num);
	if (shell->pipes == 0)
		single_cmd(shell->cmd, shell);
	else
	{
		shell->pid = ft_calloc(sizeof(int), shell->pipes + 1);
		if (!shell->pid)
			return (ft_error(1));
		multiple_cmds(shell);
	}
	return (EXIT_SUCCESS);
}
