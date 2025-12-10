/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 04:57:48 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/10 02:21:54 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	abort_heredoc(char *line, int pipe_fd[2])
{
	free(line);
	close(pipe_fd[1]);
	close(pipe_fd[0]);
	rl_event_hook = NULL;
	init_signals();
	g_signal = 0;
	return (-1);
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
}

int	heredoc_signal_check(void)
{
	if (g_signal)
		rl_done = 1;
	return (0);
}

void	check_heredoc_eof(char *line)
{
	if (!line)
	{
		ft_putstr_fd("minishell: warning: "
			"here-document delimited by end-of-file\n", 2);
		rl_on_new_line();
	}
	else
		free(line);
}
