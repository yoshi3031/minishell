/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 15:26:54 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/06 15:27:33 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredocs(t_lexer *redirections)
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

int	count_args(t_lexer *lexer_list)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = lexer_list;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->i >= 0)
		{
			i++;
			while (tmp->join_next && tmp->next && tmp->next->token != PIPE)
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (i);
}

t_cmd	*create_cmd(char **str, int num_redirections, t_lexer *redirections)
{
	t_cmd	*new_node;

	new_node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_node)
		return (0);
	new_node->str = str;
	new_node->builtin = builtin_arr(str[0]);
	new_node->num_redirections = num_redirections;
	new_node->redirections = redirections;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
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
