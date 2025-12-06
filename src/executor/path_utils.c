/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:15:54 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/06 15:06:35 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			return (ft_substr(env[i], 5, ft_strlen(env[i]) - 5));
		i++;
	}
	return (NULL);
}

static int	append_slash(char **path_str)
{
	char	*tmp;
	size_t	len;

	len = ft_strlen(*path_str);
	if (len > 0 && ft_strncmp(&(*path_str)[len - 1], "/", 1) != 0)
	{
		tmp = ft_strjoin(*path_str, "/");
		free(*path_str);
		if (!tmp)
			return (EXIT_FAILURE);
		*path_str = tmp;
	}
	return (EXIT_SUCCESS);
}

int	init_paths_from_env(t_shell *shell)
{
	char	*path_from_env;
	int		i;

	path_from_env = find_path(shell->env);
	if (path_from_env == NULL)
	{
		shell->paths = NULL;
		return (EXIT_SUCCESS);
	}
	shell->paths = ft_split(path_from_env, ':');
	free(path_from_env);
	if (shell->paths == NULL)
		return (EXIT_FAILURE);
	i = 0;
	while (shell->paths[i])
	{
		if (append_slash(&shell->paths[i]) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
