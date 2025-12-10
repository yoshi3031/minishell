/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 19:01:20 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/10 13:38:35 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_cmd(t_shell *shell)
{
	t_parser_shell	ps;
	t_cmd			*node;

	if (shell->lexer_list->token == PIPE)
		remove_node(&shell->lexer_list, shell->lexer_list->i);
	if (pipe_errors(shell, shell->lexer_list->token))
		return (EXIT_FAILURE);
	ps = init_parser_shell(shell->lexer_list, shell);
	node = init_cmd(&ps);
	if (!node)
	{
		if (g_signal)
			shell->error_num = 1;
		return (EXIT_FAILURE);
	}
	add_cmd_back(&shell->cmd, node);
	shell->lexer_list = ps.lexer_list;
	return (EXIT_SUCCESS);
}

int	parser(t_shell *shell)
{
	shell->cmd = NULL;
	count_pipes(shell->lexer_list, shell);
	if (shell->lexer_list->token == PIPE)
		return (parser_double_token_error(shell->lexer_list->token));
	while (shell->lexer_list && shell->lexer_list->token != END_OF_INPUT)
	{
		if (process_cmd(shell) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
