/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 11:44:12 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/09 11:47:03 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_cmd(t_cmd *cmd, t_shell *shell, int pipe_fd[2], int input_fd)
{
	if (cmd->prev && dup2(input_fd, STDIN_FILENO) < 0)
		ft_error(4);
	close(pipe_fd[0]);
	if (cmd->next && dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		ft_error(4);
	close(pipe_fd[1]);
	if (cmd->prev)
		close(input_fd);
	exec_cmd(cmd, shell);
}

static int	get_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

static void	handle_wait_signal(int status, int *printed_nl)
{
	if (WTERMSIG(status) == SIGINT && !*printed_nl)
	{
		write(1, "\n", 1);
		*printed_nl = 1;
	}
	else if (WTERMSIG(status) == SIGQUIT && !*printed_nl)
	{
		ft_putstr_fd("Quit: (core dumped)\n", 2);
		*printed_nl = 1;
	}
}

int	wait_all_children(int *pid, int cmd_count)
{
	int	i;
	int	status;
	int	last_status;
	int	printed_nl;

	i = 0;
	last_status = 0;
	printed_nl = 0;
	while (i < cmd_count)
	{
		if (pid[i] != -1)
		{
			waitpid(pid[i], &status, 0);
			if (WIFSIGNALED(status))
				handle_wait_signal(status, &printed_nl);
			if (i == cmd_count - 1)
				last_status = get_status(status);
		}
		i++;
	}
	return (last_status);
}
