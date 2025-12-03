/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:00:21 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 11:02:00 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*my_getenv(char **env, char *key)
{
	int		i;
	size_t	len;

	if (!env || !key)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			return (&env[i][len + 1]);
		i++;
	}
	return (NULL);
}

static char	*get_oldpwd(t_shell *shell)
{
	return (my_getenv(shell->env, "OLDPWD"));
}

static void	update_pwd_oldpwd(char *oldpwd, t_shell *shell)
{
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
		return ;
	update_or_add_env(shell, "PWD", cwd);
	update_or_add_env(shell, "OLDPWD", oldpwd);
}

static char	*get_target_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->str[1])
	{
		path = my_getenv(shell->env, "HOME");
		if (!path)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	}
	else if (ft_strcmp(cmd->str[1], "-") == 0)
	{
		path = get_oldpwd(shell);
		if (!path)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		else
			ft_putendl_fd(path, 1);
	}
	else
		path = cmd->str[1];
	return (path);
}

int	minishell_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	oldpwd[1024];

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	path = get_target_path(shell, cmd);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (1);
	}
	update_pwd_oldpwd(oldpwd, shell);
	return (0);
}
