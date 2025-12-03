/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 10:49:17 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 10:49:19 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_error(const char *key)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd((char *)key, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	is_remove_target(char *env_str, const char *key, size_t key_len)
{
	if (ft_strncmp(env_str, key, key_len) == 0 && (env_str[key_len] == '='
			|| env_str[key_len] == '\0'))
		return (1);
	return (0);
}

static char	**create_filtered_env(char **old_env, const char *key)
{
	char	**new_env;
	int		i;
	int		j;
	size_t	key_len;

	new_env = malloc(sizeof(char *) * (count_2d_arr(old_env) + 1));
	if (!new_env)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	j = 0;
	while (old_env[i])
	{
		if (is_remove_target(old_env[i], key, key_len))
			free(old_env[i]);
		else
			new_env[j++] = old_env[i];
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

void	remove_env_entry(char ***env, const char *key)
{
	char	**new_env;

	if (!env || !*env)
		return ;
	new_env = create_filtered_env(*env, key);
	if (!new_env)
		return ;
	free(*env);
	*env = new_env;
}

int	minishell_unset(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*key;

	i = 1;
	while (cmd->str[i])
	{
		key = cmd->str[i];
		if (!is_valid_key(key))
		{
			unset_error(key);
			shell->error_num = 1;
		}
		else
			remove_env_entry(&shell->env, key);
		i++;
	}
	return (0);
}
