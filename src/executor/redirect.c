/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 15:59:09 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 16:03:59 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_with_prefix(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(str);
}

static int	get_redirect_fd(t_lexer *redir)
{
	if (redir->token == REDIR_IN)
		return (open(redir->str, O_RDONLY));
	else if (redir->token == REDIR_OUT)
		return (open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (redir->token == REDIR_APPEND)
		return (open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644));
	return (-1);
}

static int	process_heredoc(t_lexer *redir)
{
	if (redir->heredoc_fd != -1)
	{
		dup2(redir->heredoc_fd, STDIN_FILENO);
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
	}
	return (EXIT_SUCCESS);
}

static int	apply_redirect(t_lexer *redir)
{
	int	fd;

	if (redir->token == HEREDOC)
		return (process_heredoc(redir));
	fd = get_redirect_fd(redir);
	if (fd < 0)
	{
		perror_with_prefix(redir->str);
		return (EXIT_FAILURE);
	}
	if (redir->token == REDIR_IN)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close(fd);
	return (EXIT_SUCCESS);
}

int	handle_redirections(t_cmd *cmd)
{
	t_lexer	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (apply_redirect(redir) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		redir = redir->next;
	}
	return (EXIT_SUCCESS);
}
