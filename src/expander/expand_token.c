/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:13:28 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 14:09:05 by yotakagi         ###   ########.fr       */
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

bool	has_quote(const char *str)
{
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
			return (true);
		str++;
	}
	return (false);
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
