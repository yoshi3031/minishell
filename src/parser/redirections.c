/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:59:59 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/10 15:01:00 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc(t_lexer *tmp, t_lexer *node, t_parser_shell *ps)
{
	bool	expand;

	if (tmp->token != HEREDOC)
		return (EXIT_SUCCESS);
	expand = (tmp->next->token = WORD);
	node->heredoc_fd = read_heredoc(tmp->next->str, expand, ps->shell->env,
			ps->shell->error_num);
	if (node->heredoc_fd == -1)
	{
		free(node->str);
		free(node);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	extract_redirection(t_lexer *tmp, t_parser_shell *parser_shell)
{
	t_lexer	*node;
	int		index_1;
	int		index_2;

	node = create_node(ft_strdup(tmp->next->str), tmp->token);
	if (!node)
		return (ft_error(1));
	if (process_heredoc(tmp, node, parser_shell) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	add_node_back(&parser_shell->redirections, node);
	index_1 = tmp->i;
	index_2 = tmp->next->i;
	remove_node(&parser_shell->lexer_list, index_1);
	remove_node(&parser_shell->lexer_list, index_2);
	parser_shell->num_redirections++;
	return (EXIT_SUCCESS);
}

int	remove_redirections(t_parser_shell *parser_shell)
{
	t_lexer	*tmp;

	while (1)
	{
		tmp = parser_shell->lexer_list;
		while (tmp && !(tmp->token >= REDIR_OUT && tmp->token <= HEREDOC)
			&& tmp->token != PIPE)
			tmp = tmp->next;
		if (!tmp || tmp->token == PIPE)
			break ;
		if (!tmp->next || tmp->next->token == END_OF_INPUT)
			return (ft_error(0));
		if (tmp->next->token >= PIPE && tmp->next->token <= HEREDOC)
			return (parser_double_token_error(tmp->next->token));
		if (extract_redirection(tmp, parser_shell) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
