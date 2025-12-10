/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/08 06:15:08 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/10 13:38:25 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_joinable(t_tokens token)
{
	return (token == WORD || token == DQUOTE_WORD || token == SQUOTE_WORD
		|| token == ENV_VAR || token == EXIT_STATUS);
}

static void	close_heredocs(t_lexer *redirections)
{
	while (redirections)
	{
		if (redirections->token == HEREDOC && redirections->heredoc_fd != -1)
		{
			close(redirections->heredoc_fd);
			redirections->heredoc_fd = -1;
		}
		redirections = redirections->next;
	}
}

void	free_cmd(t_cmd **lst)
{
	t_cmd	*tmp;
	t_cmd	*crnt;

	if (!lst || !*lst)
		return ;
	crnt = *lst;
	while (crnt)
	{
		tmp = crnt->next;
		if (crnt->redirections)
		{
			close_heredocs(crnt->redirections);
			free_lexer(&crnt->redirections);
		}
		if (crnt->str)
			free_arr(crnt->str);
		free(crnt);
		crnt = tmp;
	}
	*lst = NULL;
}

void	add_cmd_back(t_cmd **lst, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	tmp = *lst;
	if (*lst == NULL)
	{
		*lst = new_cmd;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_cmd;
	new_cmd->prev = tmp;
}
