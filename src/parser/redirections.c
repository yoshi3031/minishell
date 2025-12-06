/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:59:59 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/06 15:19:37 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_side_node(t_parser_shell *parser_shell, int index_1, int index_2)
{
	remove_node(&parser_shell->lexer_list, index_1);
	remove_node(&parser_shell->lexer_list, index_2);
}

int	extract_redirection(t_lexer *tmp, t_parser_shell *parser_shell)
{
	t_lexer	*node;
	bool	expand;

	node = create_node(ft_strdup(tmp->next->str), tmp->token);
	if (!node)
		ft_error(1);
	if (tmp->token == HEREDOC)
	{
		expand = (tmp->next->token == WORD);
		node->heredoc_fd = read_heredoc(tmp->next->str, expand,
				parser_shell->shell->env, parser_shell->shell->error_num);
		if (node->heredoc_fd == -1)
		{
			free(node->str);
			free(node);
			return (EXIT_FAILURE);
		}
	}
	add_node_back(&parser_shell->redirections, node);
	remove_side_node(parser_shell, tmp->i, tmp->next->i);
	parser_shell->num_redirections++;
	return (EXIT_SUCCESS);
}

int	remove_redirections(t_parser_shell *parser_shell)
{
	t_lexer	*tmp;

	tmp = parser_shell->lexer_list;
	while (tmp && !(tmp->token >= REDIR_OUT && tmp->token <= HEREDOC))
	{
		if (tmp->token == PIPE)
			break ;
		tmp = tmp->next;
	}
	if (!tmp || tmp->token == PIPE)
		return (EXIT_SUCCESS);
	if (!tmp->next || tmp->next->token == END_OF_INPUT)
		ft_error(0);
	if (tmp->next->token >= PIPE && tmp->next->token <= HEREDOC)
		parser_double_token_error(tmp->next->token);
	if (tmp->token >= REDIR_OUT && tmp->token <= HEREDOC)
	{
		if (extract_redirection(tmp, parser_shell) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (remove_redirections(parser_shell) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
