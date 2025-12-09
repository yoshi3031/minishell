/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 10:49:17 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:09:35 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 環境変数エントリが削除対象のキーと一致するかどうかを判定する
// `KEY=value`と`KEY`の両方の形式を考慮する
// @param env_str: 環境変数エントリの文字列
// @param key: 削除するキー
// @param key_len: 削除するキーの長さ
// @return: 一致する場合は1、しない場合は0
static int	is_remove_target(char *env_str, const char *key, size_t key_len)
{
	// キーが前方一致し、かつその直後が'='か文字列終端であれば一致とみなす
	if (ft_strncmp(env_str, key, key_len) == 0 && (env_str[key_len] == '='
			|| env_str[key_len] == '\0'))
		return (1);
	return (0);
}

// 指定されたキーを除外した新しい環境変数配列を作成する
// @param old_env: 元の環境変数配列
// @param key: 除外するキー
// @return: 新しい環境変数配列
static char	**create_filtered_env(char **old_env, const char *key)
{
	char	**new_env;
	int		i;
	int		j;
	size_t	key_len;

	// 元の配列と同じサイズのメモリを確保（最大で同じサイズになるため）
	new_env = malloc(sizeof(char *) * (count_2d_arr(old_env) + 1));
	if (!new_env)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	j = 0;
	// 元の配列を走査
	while (old_env[i])
	{
		// 削除対象のキーと一致するか判定
		if (is_remove_target(old_env[i], key, key_len))
			free(old_env[i]);
		else
			// 削除対象でなければ、新しい配列にポインタをコピー
			new_env[j++] = old_env[i];
		i++;
	}
	new_env[j] = NULL;
	return (new_env);
}

// 環境変数から指定されたキーのエントリを削除する
// @param env: 環境変数配列へのポインタ
// @param key: 削除するキー
void	remove_env_entry(char ***env, const char *key)
{
	char	**new_env;

	if (!env || !*env)
		return ;
	// 指定されたキーを除外した新しい配列を作成
	new_env = create_filtered_env(*env, key);
	if (!new_env)
		return ;
	// 古い配列のポインタ自体を解放
	free(*env);
	// 環境変数ポインタを新しい配列に更新
	*env = new_env;
}

// unsetコマンドを実行する
// 指定された各キーの環境変数を削除する
// @param shell: シェルの状態を保持する構造体
// @param cmd: コマンドの情報を保持する構造体
// @return: 常に0
int	minishell_unset(t_shell *shell, t_cmd *cmd)
{
	int		i;
	char	*key;

	i = 1;
	// コマンド引数をループで処理
	while (cmd->str[i])
	{
		key = cmd->str[i];
		// 各キーについて環境変数から削除
		remove_env_entry(&shell->env, key);
		i++;
	}
	return (0);
}
