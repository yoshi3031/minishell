/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:48:43 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 11:55:40 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_key(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	sort_env_ascii(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**dup_and_sort_env(char **env)
{
	char	**copy;
	int		size;
	int		i;

	size = count_2d_arr(env);
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < size)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	sort_env_ascii(copy);
	return (copy);
}

void	print_export_line(char *env_entry)
{
	char	*eq;
	int		key_len;

	ft_putstr_fd("declare -x ", 1);
	eq = ft_strchr(env_entry, '=');
	if (eq)
	{
		key_len = eq - env_entry;
		write(1, env_entry, key_len + 1);
		write(1, "\"", 1);
		ft_putstr_fd(eq + 1, 1);
		write(1, "\"\n", 2);
	}
	else
	{
		ft_putstr_fd(env_entry, 1);
		write(1, "\n", 1);
	}
}

void	print_env_sorted(char **env)
{
	char	**copy;
	int		i;

	copy = dup_and_sort_env(env);
	if (!copy)
		return ;
	i = 0;
	while (copy[i])
	{
		print_export_line(copy[i]);
		i++;
	}
	free_2d_arr(copy);
}
