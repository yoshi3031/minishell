/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_until.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:00:21 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 16:02:05 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

static void	update_exit_status(int status, int *last_status_code,
		int *printed_nl)
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
	if (WIFEXITED(status))
		*last_status_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		*last_status_code = 128 + WTERMSIG(status);
}

int	wait_all_children(int *pid, int cmd_count)
{
	int	i;
	int	status;
	int	last_status_code;
	int	last_pid;
	int	printed_nl;

	printed_nl = 0;
	last_status_code = 0;
	last_pid = pid[cmd_count - 1];
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pid[i], &status, 0);
		if (pid[i] == last_pid || WIFSIGNALED(status))
			update_exit_status(status, &last_status_code, &printed_nl);
		i++;
	}
	return (last_status_code);
}
