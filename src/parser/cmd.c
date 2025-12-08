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

static int	merge_tokens(t_lexer *tmp)
{
	t_lexer	*next;
	char	*joined;

	next = tmp->next;
	joined = ft_strjoin(tmp->str, next->str);
	if (!joined)
		return (EXIT_FAILURE);
	free(tmp->str);
	tmp->str = joined;
	tmp->join_next = next->join_next;
	if (next->token == DQUOTE_WORD || next->token == SQUOTE_WORD)
		tmp->token = DQUOTE_WORD;
	tmp->next = next->next;
	if (next->next)
		next->next->prev = tmp;
	free(next->str);
	free(next);
	return (EXIT_SUCCESS);
}

static void	unite_tokens(t_lexer **lexer_list)
{
	t_lexer	*tmp;

	if (!lexer_list || !*lexer_list)
		return ;
	tmp = *lexer_list;
	while (tmp)
	{
		while (tmp->join_next && tmp->next && is_joinable(tmp->token))
		{
			if (merge_tokens(tmp) == EXIT_FAILURE)
				return ;
		}
		tmp = tmp->next;
	}
}

static int	count_args(t_lexer *lexer_list)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = lexer_list;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->i >= 0)
			i++;
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

t_cmd	*init_cmd(t_parser_shell *parser_shell)
{
	int		arg_size;
	int		i;
	t_lexer	*tmp;
	char	**str;

	unite_tokens(&parser_shell->lexer_list);
	if (remove_redirections(parser_shell) == EXIT_FAILURE)
		return (NULL);
	arg_size = count_args(parser_shell->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		ft_error(1);
	tmp = parser_shell->lexer_list;
	i = 0;
	while (arg_size-- > 0)
	{
		if (tmp && tmp->str)
		{
			str[i++] = ft_strdup(tmp->str);
			remove_node(&parser_shell->lexer_list, tmp->i);
		}
		tmp = parser_shell->lexer_list;
	}
	return (create_cmd(str, parser_shell->num_redirections,
			parser_shell->redirections));
}
