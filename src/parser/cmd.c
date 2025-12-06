/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhara <nhara@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 09:06:44 by nagisa            #+#    #+#             */
/*   Updated: 2025/05/22 10:45:01 by nhara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO

static int	is_joinable(t_tokens token)
{
	return (token == WORD || token == DQUOTE_WORD || token == SQUOTE_WORD
		|| token == ENV_VAR || token == EXIT_STATUS);
}

static int	join_process(t_lexer *tmp, t_lexer *next)
{
	char	*joined;

	if (!tmp->str || !next->str)
		return (0);
	joined = ft_strjoin(tmp->str, next->str);
	if (!joined)
		return (-1);
	free(tmp->str);
	tmp->str = joined;
	tmp->join_next = next->join_next;
	if (next->token == DQUOTE_WORD || next->token == SQUOTE_WORD)
		tmp->token = DQUOTE_WORD;
	tmp->next = next->next;
	if (next->next)
		next->next->prev = tmp;
	if (next->str)
		free(next->str);
	free(next);
	return (1);
}

static void	unite_tokens(t_lexer **lexer_list)
{
	t_lexer	*tmp;
	int		status;

	if (!lexer_list || !*lexer_list)
		return ;
	tmp = *lexer_list;
	while (tmp)
	{
		while (tmp->join_next && tmp->next && is_joinable(tmp->token)
			&& tmp->next->token != PIPE && tmp->next->token != END_OF_INPUT)
		{
			status = join_process(tmp, tmp->next);
			if (status == 0)
				break ;
			if (status == -1)
				return ;
		}
		tmp = tmp->next;
	}
}

t_cmd	*init_cmd(t_parser_shell *parser_shell)
{
	int		arg_size;
	int		i;
	char	**str;

	unite_tokens(&parser_shell->lexer_list);
	if (remove_redirections(parser_shell) == EXIT_FAILURE)
		return (NULL);
	arg_size = count_args(parser_shell->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		ft_error(1);
	i = 0;
	while (arg_size > 0)
	{
		if (parser_shell->lexer_list && parser_shell->lexer_list->str)
		{
			str[i++] = ft_strdup(parser_shell->lexer_list->str);
			remove_node(&parser_shell->lexer_list, parser_shell->lexer_list->i);
		}
	}
	return (create_cmd(str, parser_shell->num_redirections,
			parser_shell->redirections));
}
