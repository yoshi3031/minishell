/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:50:10 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 12:08:55 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**realloc_env(char **old_env, char *new_entry)
{
	int		i;
	char	**new_env;
	int		j;

	j = 0;
	i = 0;
	while (old_env && old_env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (old_env && old_env[i])
	{
		new_env[j++] = ft_strdup(old_env[i]);
		i++;
	}
	new_env[j++] = new_entry;
	new_env[j] = NULL;
	free_2d_arr(old_env);
	return (new_env);
}

void	update_or_add_env(t_shell *shell, char *key, char *val)
{
	int		i;
	size_t	key_len;
	char	*entry;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	entry = ft_strjoin(tmp, val);
	free(tmp);
	key_len = ft_strlen(key);
	i = 0;
	while (shell->env[i])
	{
		if (!ft_strncmp(shell->env[i], key, key_len)
			&& shell->env[i][key_len] == '=')
		{
			free(shell->env[i]);
			shell->env[i] = entry;
			return ;
		}
		i++;
	}
	shell->env = realloc_env(shell->env, entry);
}

void	process_export_no_value(t_shell *shell, char *arg)
{
	char	*key;
	char	*val;

	key = ft_strdup(arg);
	if (!is_valid_key(key))
		export_error(arg);
	if (find_env_entry(shell->env, key))
	{
		free(key);
		return ;
	}
	else
	{
		val = ft_strdup("");
		update_or_add_env(shell, key, val);
		free(val);
	}
	free(key);
}

void	process_export_arg(t_shell *shell, char *arg)
{
	char	*key;
	char	*val;

	if (ft_strchr(arg, '='))
	{
		key = get_key(arg);
		val = get_val(arg);
		if (!is_valid_key(key))
			export_error(arg);
		else
			update_or_add_env(shell, key, val);
		free(key);
		free(val);
	}
	else
		process_export_no_value(shell, arg);
}

int	minishell_export(t_shell *shell, t_cmd *cmd)
{
	int		i;
	int		status;
	char	*key_check;

	status = 0;
	if (!cmd->str[1])
	{
		print_env_sorted(shell->env);
		return (0);
	}
	i = 1;
	while (cmd->str[i])
	{
		process_export_arg(shell, cmd->str[i]);
		key_check = get_key(cmd->str[i]);
		if (!key_check)
			key_check = ft_strdup(cmd->str[i]);
		if (!is_valid_key(key_check))
			status = 1;
		free(key_check);
		i++;
	}
	return (status);
}
