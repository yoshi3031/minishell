/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:20:47 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 05:06:41 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_line(char *line, int fd, t_shell *shell, bool expand)
{
	char	*expanded;

	if (expand)
	{
		expanded = expand_heredoc_line(line, shell->env, shell->error_num);
		ft_putendl_fd(expanded, fd);
		free(expanded);
	}
	else
		ft_putendl_fd(line, fd);
}

static int	heredoc_loop(int pfd[2], const char *delim,
		t_shell *shell, bool expand)
{
	char	*line;

	setup_heredoc_signals();
	rl_event_hook = heredoc_signal_check;
	while (1)
	{
		line = readline("> ");
		if (g_signal)
			return (abort_heredoc(line, pfd));
		if (!line || ft_strcmp(line, delim) == 0)
			break ;
		process_line(line, pfd[1], shell, expand);
		free(line);
	}
	rl_event_hook = NULL;
	init_signals();
	check_heredoc_eof(line);
	return (close(pfd[1]), pfd[0]);
}

int	read_heredoc(const char *delimiter, bool expand, char **env, int status)
{
	int		pfd[2];
	t_shell	tmp_shell;

	tmp_shell.env = env;
	tmp_shell.error_num = status;
	if (pipe(pfd) == -1)
		return (perror("minishell: pipe"), -1);
	return (heredoc_loop(pfd, delimiter, &tmp_shell, expand));
}
