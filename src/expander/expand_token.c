/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:13:28 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 12:43:08 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_all_cmds(t_cmd *cmds, char **env, int last_status)
{
	t_cmd	*cur;

	cur = cmds;
	while (cur)
	{
		expand_cmd(cur, env, last_status);
		cur = cur->next;
	}
}

void	expand_all_redirs(t_lexer *redir, char **env, int last_status)
{
	char	*expanded;

	while (redir)
	{
		if (redir->token == HEREDOC)
		{
			redir = redir->next;
			continue ;
		}
		expanded = expand_token(redir->str, env, last_status);
		if (redir->str)
			free(redir->str);
		redir->str = expanded;
		redir = redir->next;
	}
}

void	expand_all_tokens(char **args, char **env, int last_status)
{
	int		i;
	char	*expanded;

	i = 0;
	while (args && args[i])
	{
		expanded = expand_token(args[i], env, last_status);
		if (args[i])
			free(args[i]);
		args[i] = expanded;
		i++;
	}
}

static void	fill_expanded_str(t_expand *e, char **env, int last_status)
{
	while (e->input[e->i])
	{
		if (e->input[e->i] == '\'' && !e->in_double_quote)
		{
			e->in_single_quote = !e->in_single_quote;
			e->i++;
		}
		else if (e->input[e->i] == '"' && !e->in_single_quote)
		{
			e->in_double_quote = !e->in_double_quote;
			e->i++;
		}
		else if (e->input[e->i] == '$' && !e->in_single_quote)
			handle_dollar(e, env, last_status);
		else
			e->result[e->j++] = e->input[e->i++];
	}
	e->result[e->j] = '\0';
}

char	*expand_token(const char *input, char **env, int last_status)
{
	t_expand	e;
	size_t		len;

	if (!input)
		return (NULL);
	init_expand(&e, input);
	len = calc_expanded_length(input, env, last_status);
	e.result = malloc(len + 1);
	if (!e.result)
		return (NULL);
	fill_expanded_str(&e, env, last_status);
	return (e.result);
}
