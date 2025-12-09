/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:48:43 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:09:34 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 環境変数のキーとして有効かどうかをチェックする
// キーはアルファベットまたはアンダースコアで始まり、
// それ以降はアルファベット、数字、アンダースコアのみ含むことができる
// @param key: チェックするキー文字列
// @return: 有効な場合は1、無効な場合は0
int	is_valid_key(const char *key)
{
	int	i;

	// キーがNULL、または先頭が英字でもアンダースコアでもない場合は無効
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		// 2文字目以降が英数字またはアンダースコアでない場合は無効
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// 環境変数配列をASCII順でソートする（バブルソート）
// @param env: ソートする環境変数配列
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
			// 文字列を比較し、順序が逆なら入れ替える
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

// 環境変数配列を複製し、ソートして返す
// @param env: 元の環境変数配列
// @return: 複製・ソートされた新しい環境変数配列
char	**dup_and_sort_env(char **env)
{
	char	**copy;
	int		size;
	int		i;

	size = count_2d_arr(env);
	// 元の配列と同じサイズのメモリを確保
	copy = malloc(sizeof(char *) * (size + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < size)
	{
		// 各エントリを複製してコピー配列に格納
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	sort_env_ascii(copy);
	return (copy);
}

// `export`コマンドの出力形式で環境変数エントリを1行表示する
// `declare -x KEY="VALUE"` または `declare -x KEY` の形式
// @param env_entry: 表示する環境変数エントリ
void	print_export_line(char *env_entry)
{
	char	*eq;
	int		key_len;

	ft_putstr_fd("declare -x ", 1);
	// '='の位置を探す
	eq = ft_strchr(env_entry, '=');
	if (eq)
	{
		// KEY="VALUE" 形式で出力
		key_len = eq - env_entry;
		write(1, env_entry, key_len + 1);
		write(1, "\"", 1);
		ft_putstr_fd(eq + 1, 1);
		write(1, "\"\n", 2);
	}
	else
	{
		// KEY のみ（値なし）の形式で出力
		ft_putstr_fd(env_entry, 1);
		write(1, "\n", 1);
	}
}

// 環境変数をソートして`export`形式で表示する（引数なしの`export`の動作）
// @param env: 環境変数配列
void	print_env_sorted(char **env)
{
	char	**copy;
	int		i;

	// 環境変数を複製してソート
	copy = dup_and_sort_env(env);
	if (!copy)
		return ;
	i = 0;
	while (copy[i])
	{
		// 各エントリをexport形式で表示
		print_export_line(copy[i]);
		i++;
	}
	free_2d_arr(copy);
}
