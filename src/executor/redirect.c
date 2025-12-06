/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 09:05:10 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/06 15:06:55 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_with_prefix(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(str);
}

int	handle_redirections(t_cmd *cmd)
{
	t_lexer	*redir;
	int		fd;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->token == REDIR_IN)
		{
			fd = open(redir->str, O_RDONLY);
			if (fd < 0)
				return (perror_with_prefix(redir->str), EXIT_FAILURE);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->token == HEREDOC)
		{
			if (redir->heredoc_fd != -1)
			{
				dup2(redir->heredoc_fd, STDIN_FILENO);
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
		}
		else if (redir->token == REDIR_OUT)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				return (perror_with_prefix(redir->str), EXIT_FAILURE);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->token == REDIR_APPEND)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
				return (perror_with_prefix(redir->str), EXIT_FAILURE);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
	return (EXIT_SUCCESS);
}
