/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 08:48:31 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/09 11:57:08 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>

static int	ft_fork(t_shell *shell, int pfd[2], int input_fd, int i)
{
	shell->pid[i] = fork();
	if (shell->pid[i] < 0)
		ft_error(3);
	if (shell->pid[i] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup_cmd(shell->cmd, shell, pfd, input_fd);
	}
	return (EXIT_SUCCESS);
}

static int	close_parent_fds(t_shell *shell, int pfd[2], int input_fd)
{
	close(pfd[1]);
	if (shell->cmd->prev)
		close(input_fd);
	if (shell->cmd->next)
		return (pfd[0]);
	return (-1);
}

static t_cmd	*get_cmdlist_first(t_cmd *node)
{
	if (!node)
		return (NULL);
	while (node->prev != NULL)
		node = node->prev;
	return (node);
}

int	multiple_cmds(t_shell *shell)
{
	int	pfd[2];
	int	input_fd;
	int	i;

	input_fd = STDIN_FILENO;
	i = 0;
	while (shell->cmd)
	{
		if (shell->cmd->next)
		{
			if (pipe(pfd) < 0)
				return (ft_error(5));
		}
		ft_fork(shell, pfd, input_fd, i++);
		input_fd = close_parent_fds(shell, pfd, input_fd);
		if (!shell->cmd->next)
			break ;
		shell->cmd = shell->cmd->next;
	}
	signal(SIGINT, SIG_IGN);
	shell->error_num = wait_all_children(shell->pid, shell->pipes + 1);
	init_signals();
	shell->cmd = get_cmdlist_first(shell->cmd);
	return (0);
}
