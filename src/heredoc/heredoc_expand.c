/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 03:02:10 by ayamamot          #+#    #+#             */
/*   Updated: 2025/12/10 13:37:31 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_expand_value(char *str, int *i, char **env, int status)
{
	int		len;
	char	*key;
	char	*val;

	if (str[*i + 1] == '?')
	{
		*i += 2;
		return (ft_itoa(status));
	}
	*i += 1;
	len = 0;
	while (str[*i + len] && (ft_isalnum(str[*i + len]) || str[*i + len] == '_'))
		len++;
	key = ft_substr(str, *i, len);
	val = get_env_value(env, key);
	free(key);
	*i += len;
	return (val);
}

static void	append_value(char *dst, size_t *current_len, char *val)
{
	size_t	val_len;

	if (!val)
		return ;
	val_len = ft_strlen(val);
	if (dst)
		ft_strlcpy(dst + *current_len, val, val_len + 1);
	*current_len += val_len;
}

static size_t	perform_expansion(char *line, char **env, int status, char *dst)
{
	size_t	len;
	int		i;
	char	*val;

	len = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && (line[i + 1] == '?' || ft_isalpha(line[i + 1])
				|| line[i + 1] == '_'))
		{
			val = get_expand_value(line, &i, env, status);
			append_value(dst, &len, val);
			free(val);
		}
		else
		{
			if (dst)
				dst[len] = line[i];
			len++;
			i++;
		}
	}
	return (len);
}

char	*expand_heredoc_line(char *line, char **env, int status)
{
	char	*new_line;
	size_t	len;

	len = perform_expansion(line, env, status, NULL);
	new_line = (char *)malloc(sizeof(char) * (len + 1));
	if (!new_line)
		return (NULL);
	perform_expansion(line, env, status, new_line);
	new_line[len] = '\0';
	return (new_line);
}
