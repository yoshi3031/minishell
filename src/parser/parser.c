/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 19:01:20 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/06 15:23:35 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	node_error(t_shell *shell)
{
	if (g_signal)
	{
		shell->error_num = 1;
		return (EXIT_FAILURE);
	}
	ft_error(0);
	return (EXIT_FAILURE);
}

int	parser(t_shell *shell)
{
	t_cmd			*node;
	t_parser_shell	parser_shell;

	shell->cmd = NULL;
	count_pipes(shell->lexer_list, shell);
	if (shell->lexer_list->token == PIPE)
		return (parser_double_token_error(shell->lexer_list->token));
	while (shell->lexer_list && shell->lexer_list->token != END_OF_INPUT)
	{
		if (shell->lexer_list && shell->lexer_list->token == PIPE)
			remove_node(&shell->lexer_list, shell->lexer_list->i);
		if (pipe_errors(shell, shell->lexer_list->token))
			return (EXIT_FAILURE);
		parser_shell = init_parser_shell(shell->lexer_list, shell);
		node = init_cmd(&parser_shell);
		if (!node)
			node_error(shell);
		if (!shell->cmd)
			shell->cmd = node;
		else
			add_cmd_back(&shell->cmd, node);
		shell->lexer_list = parser_shell.lexer_list;
	}
	return (EXIT_SUCCESS);
}
