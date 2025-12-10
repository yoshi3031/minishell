/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:03:54 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/10 13:37:27 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_expand(t_expand *e, const char *input)
{
	e->i = 0;
	e->j = 0;
	e->in_single_quote = false;
	e->in_double_quote = false;
	e->input = input;
	e->result = NULL;
}

void	handle_dollar(t_expand *e, char **env, int last_status)
{
	char	var[MAX_VAR_NAME];
	size_t	k;

	e->i++;
	if (e->input[e->i] == '?')
	{
		handle_exit_status(e, last_status);
		e->i++;
	}
	else if (ft_isalpha(e->input[e->i]) || e->input[e->i] == '_')
	{
		k = 0;
		while (ft_isalnum(e->input[e->i]) || e->input[e->i] == '_')
			var[k++] = e->input[e->i++];
		var[k] = '\0';
		copy_env_value(e, env, var);
	}
	else
		e->result[e->j++] = '$';
}

void	copy_env_value(t_expand *e, char **env, const char *key)
{
	char	*val;

	val = get_env_value(env, key);
	if (val)
	{
		e->j += ft_strlcpy(e->result + e->j, val, ft_strlen(val) + 1);
		free(val);
	}
}

void	handle_exit_status(t_expand *e, int last_status)
{
	char	*num;

	num = ft_itoa(last_status);
	if (!num)
		exit(EXIT_FAILURE);
	e->j += ft_strlcpy(e->result + e->j, num, ft_strlen(num) + 1);
	free(num);
}

char	*get_env_value(char **env, const char *key)
{
	int		i;
	size_t	len;

	if (!key || !env)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}
