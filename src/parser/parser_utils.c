/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:04:52 by nhara             #+#    #+#             */
/*   Updated: 2025/12/10 13:38:34 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// remove comment

#include "minishell.h"

void	free_lexer(t_lexer **lst)
{
	t_lexer	*tmp;

	if (!*lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->str)
			free((*lst)->str);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	count_pipes(t_lexer *lexer_list, t_shell *shell)
{
	t_lexer	*tmp;

	tmp = lexer_list;
	shell->pipes = 0;
	while (tmp)
	{
		if (tmp->token == PIPE)
			shell->pipes++;
		tmp = tmp->next;
	}
}

t_parser_shell	init_parser_shell(t_lexer *lexer_list, t_shell *shell)
{
	t_parser_shell	parser_shell;

	parser_shell.lexer_list = lexer_list;
	parser_shell.redirections = NULL;
	parser_shell.num_redirections = 0;
	parser_shell.shell = shell;
	return (parser_shell);
}
