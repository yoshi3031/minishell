/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_len.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:04:27 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 15:03:14 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_status_len(int status)
{
	size_t	len;

	len = 1;
	while (status >= 10)
	{
		status /= 10;
		len++;
	}
	return (len);
}

size_t	get_env_len(char **env, const char *key)
{
	char	*val;
	size_t	len;
	char	*dup;

	dup = ft_strdup(key);
	if (!dup)
		return (0);
	val = get_env_value(env, dup);
	free(dup);
	if (!val)
		return (0);
	len = ft_strlen(val);
	free(val);
	return (len);
}

size_t	expand_dollar_len(const char *s, size_t *i, char **env, int last_status)
{
	size_t	start;
	size_t	len;
	char	*key;

	len = 0;
	if (s[*i] == '?')
	{
		len = count_status_len(last_status);
		(*i)++;
	}
	else if (ft_isalpha(s[*i]) || s[*i] == '_')
	{
		start = *i;
		while (ft_isalnum(s[*i]) || s[*i] == '_')
			(*i)++;
		key = ft_substr(s, start, *i - start);
		if (!key)
			return (0);
		len = get_env_len(env, key);
		free(key);
	}
	else
		len++;
	return (len);
}

void	update_quote_state(char c, bool *in_squote, bool *in_dquote)
{
	if (c == '\'' && !*in_dquote)
		*in_squote = !*in_squote;
	else if (c == '"' && !*in_squote)
		*in_dquote = !*in_dquote;
}

size_t	calc_expanded_length(const char *s, char **env, int last_status)
{
	size_t	len;
	size_t	i;
	bool	in_squote;
	bool	in_dquote;

	len = 0;
	i = 0;
	in_squote = false;
	in_dquote = false;
	while (s[i])
	{
		update_quote_state(s[i], &in_squote, &in_dquote);
		if (s[i] == '$' && !in_squote)
		{
			i++;
			len += expand_dollar_len(s, &i, env, last_status);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
