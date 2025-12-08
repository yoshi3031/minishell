/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:00:21 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 13:00:26 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 環境変数から指定されたキーに対応する値を取得する
// `key=` の形式で変数を探し、その直後から始まる値を返す
// @param env: 環境変数の配列
// @param key: 検索するキー
// @return: キーに対応する値のポインタ。見つからない場合はNULL
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

// OLDPWD環境変数の値を取得する
static char	*get_oldpwd(t_shell *shell)
{
	return (my_getenv(shell->env, "OLDPWD"));
}

// PWDとOLDPWD環境変数を更新する
// @param oldpwd: cdコマンド実行前のカレントディレクトリ
// @param shell: シェルの状態を保持する構造体
static void	update_pwd_oldpwd(char *oldpwd, t_shell *shell)
{
	char	cwd[1024];

	if (!getcwd(cwd, sizeof(cwd)))
		return ;
	update_or_add_env(shell, "PWD", cwd);
	update_or_add_env(shell, "OLDPWD", oldpwd);
}

// cdコマンドの移動先パスを取得する
// 引数がない場合はHOME、"-"の場合はOLDPWD、それ以外は指定されたパス
// @param shell: シェルの状態を保持する構造体
// @param cmd: コマンドの情報を保持する構造体
// @return: 移動先のパス
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

// cdコマンドを実行する
// @param shell: シェルの状態を保持する構造体
// @param cmd: コマンドの情報を保持する構造体
// @return: 成功した場合は0、失敗した場合は1
int	minishell_cd(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	oldpwd[1024];

	if (cmd->str[1] && cmd->str[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", 2);
		return (1);
	}
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
