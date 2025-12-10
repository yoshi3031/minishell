/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lexer_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhara <nhara@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:53:20 by nhara             #+#    #+#             */
/*   Updated: 2025/05/22 10:01:23 by nhara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// +---+      +---+      +---+
// | A | <--> | B | <--> | C |
// +---+      +---+      +---+

t_lexer	*remove_one_node(t_lexer **lst)
{
	if ((*lst)->str)
	{
		free((*lst)->str);
		(*lst)->str = NULL;
	}
	free(*lst);
	*lst = NULL;
	return (NULL);
}

void	remove_first_node(t_lexer **lst)
{
	t_lexer	*node;

	node = *lst;
	*lst = node->next;
	remove_one_node(&node);
	if (*lst)
		(*lst)->prev = NULL;
}

void	remove_node(t_lexer **lst, int target_i)
{
	t_lexer	*node;
	t_lexer	*prev;
	t_lexer	*start;

	start = *lst;
	node = start;
	if ((*lst)->i == target_i)
	{
		remove_first_node(lst);
		return ;
	}
	while (node && node->i != target_i)
	{
		prev = node;
		node = node->next;
	}
	if (node)
		prev->next = node->next;
	else
		prev->next = NULL;
	if (prev->next)
		prev->next->prev = prev;
	remove_one_node(&node);
	*lst = start;
}

t_lexer	*create_node(char *str, int token)
{
	t_lexer	*new_node;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (0);
	new_node->str = str;
	new_node->token = token;
	new_node->i = -1;
	new_node->heredoc_fd = -1;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	add_node_back(t_lexer **lst, t_lexer *new_node)
{
	t_lexer	*tmp;

	tmp = *lst;
	if (*lst == NULL)
	{
		*lst = new_node;
		new_node->i = 0;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_node;
	new_node->prev = tmp;
	new_node->i = tmp->i + 1;
}
