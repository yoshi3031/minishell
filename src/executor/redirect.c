/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 09:05:10 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/10 13:37:10 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_with_prefix(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(str);
}

static int	handle_input(t_lexer *redir)
{
	int	fd;

	if (redir->token == HEREDOC)
	{
		if (redir->heredoc_fd != -1)
		{
			dup2(redir->heredoc_fd, STDIN_FILENO);
			close(redir->heredoc_fd);
			redir->heredoc_fd = -1;
		}
		return (EXIT_SUCCESS);
	}
	fd = open(redir->str, O_RDONLY);
	if (fd < 0)
	{
		perror_with_prefix(redir->str);
		return (EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (EXIT_SUCCESS);
}

static int	handle_output(t_lexer *redir)
{
	int	fd;
	int	flags;

	if (redir->token == REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(redir->str, flags, 0644);
	if (fd < 0)
	{
		perror_with_prefix(redir->str);
		return (EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (EXIT_SUCCESS);
}

int	handle_redirections(t_cmd *cmd)
{
	t_lexer	*redir;
	int		status;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->token == REDIR_IN || redir->token == HEREDOC)
			status = handle_input(redir);
		else if (redir->token == REDIR_OUT || redir->token == REDIR_APPEND)
			status = handle_output(redir);
		else
			status = EXIT_SUCCESS;
		if (status == EXIT_FAILURE)
			return (EXIT_FAILURE);
		redir = redir->next;
	}
	return (EXIT_SUCCESS);
}
