/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 05:27:29 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 11:07:52 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// envコマンドを実行する
// '='を含む環境変数のみ（つまり、値が設定されている変数）を出力する
// @param shell: シェルの状態を保持する構造体
// @param cmd: コマンドの情報を保持する構造体（この関数では未使用）
// @return: 常に0
int	minishell_env(t_shell *shell, t_cmd *cmd)
{
	int	i;

	(void)cmd;
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strchr(shell->env[i], '='))
		{
			ft_putstr_fd(shell->env[i], 1);
			write(1, "\n", 1);
		}
		i++;
	}
	return (0);
}

// 環境変数に指定されたキーが存在するかどうかを検索する
// `KEY=value` 形式と、`KEY`のみの形式の両方をチェックする
// @param env: 環境変数の配列
// @param key: 検索するキー
// @return: キーが見つかった場合は1、見つからない場合は0
int	find_env_entry(char **env, char *key)
{
	int		i;
	size_t	len;

	if (!env || !key)
		return (0);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, len) == 0)
		{
			if (env[i][len] == '=' || env[i][len] == '\0')
				return (1);
		}
		i++;
	}
	return (0);
}
