/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 10:50:10 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 17:24:03 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 環境変数配列を再確保し、新しいエントリを追加する
// @param old_env: 古い環境変数配列
// @param new_entry: 追加する新しい環境変数の文字列
// @return: 新しい環境変数配列
char	**realloc_env(char **old_env, char *new_entry)
{
	int		i;
	char	**new_env;
	int		j;

	j = 0;
	i = 0;
	// 元の配列の要素数を数える
	while (old_env && old_env[i])
		i++;
	// 新しい配列のために、元のサイズ+新しいエントリ(1)+NULL終端(1)のメモリを確保
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	// 古い配列の内容を新しい配列に複製
	while (old_env && old_env[i])
	{
		new_env[j++] = ft_strdup(old_env[i]);
		i++;
	}
	// 新しいエントリを追加し、配列をNULLで終端
	new_env[j++] = new_entry;
	new_env[j] = NULL;
	// 古い配列のポインタ自体ではなく、その中身を解放
	free_2d_arr(old_env);
	return (new_env);
}

// 環境変数を更新または追加する
// @param shell: シェルの状態を保持する構造体
// @param key: 環境変数のキー
// @param val: 環境変数の値
void	update_or_add_env(t_shell *shell, char *key, char *val)
{
	int		i;
	size_t	key_len;
	char	*entry;
	char	*tmp;

	// "key=val" 形式の文字列を作成
	tmp = ft_strjoin(key, "=");
	entry = ft_strjoin(tmp, val);
	free(tmp);
	key_len = ft_strlen(key);
	i = 0;
	// 既存の環境変数を探す
	while (shell->env[i])
	{
		// キーが一致するかどうかをチェック
		if (!ft_strncmp(shell->env[i], key, key_len)
			&& shell->env[i][key_len] == '=')
		{
			// キーが見つかった場合、古いエントリを解放し、新しいエントリで上書き
			free(shell->env[i]);
			shell->env[i] = entry;
			return ;
		}
		i++;
	}
	// キーが見つからなかった場合、新しいエントリとして配列の末尾に追加
	shell->env = realloc_env(shell->env, entry);
}

// 値なしのexport引数を処理する（例: `export VAR`）
// キーが有効であれば、値が空文字列の環境変数として追加する
// @param shell: シェルの状態を保持する構造体
// @param arg: exportの引数
void	process_export_no_value(t_shell *shell, char *arg)
{
	char	*key;
	char	*val;

	key = ft_strdup(arg);
	// キーが有効な識別子かチェック
	if (!is_valid_key(key))
	{
		export_error(arg);
		free(key);
		return ;
	}
	// すでに環境変数が存在するかチェック
	if (find_env_entry(shell->env, key))
	{
		// 存在すれば何もしない
		free(key);
		return ;
	}
	else
	{
		// 存在しなければ、値が空文字列の変数として追加
		val = ft_strdup("");
		update_or_add_env(shell, key, val);
		free(val);
	}
	free(key);
}

// exportコマンドの単一の引数を処理する
// '='が含まれているかどうかで、値あり・値なしの処理を分岐する
// @param shell: シェルの状態を保持する構造体
// @param arg: 処理する引数
void	process_export_arg(t_shell *shell, char *arg)
{
	char	*key;
	char	*val;

	// 引数に'='が含まれているかチェック
	if (ft_strchr(arg, '='))
	{
		// キーと値を取得
		key = get_key(arg);
		val = get_val(arg);
		if (!is_valid_key(key))
			// キーが無効ならエラー
			export_error(arg);
		else
			update_or_add_env(shell, key, val);
		free(key);
		free(val);
	}
	else
		// '='が含まれていない場合は、値なしの引数として処理
		process_export_no_value(shell, arg);
}

// exportコマンドを実行する
// - 引数なし: 環境変数をソートして表示
// - 引数あり: 各引数を処理して環境変数を設定
// @param shell: シェルの状態を保持する構造体
// @param cmd: コマンドの情報を保持する構造体
// @return: 成功した場合は0、無効なキーがあった場合は1
int	minishell_export(t_shell *shell, t_cmd *cmd)
{
	int		i;
	int		status;
	char	*key_check;

	status = 0;
	// 引数がない場合、ソートされた環境変数リストを表示
	if (!cmd->str[1])
	{
		print_env_sorted(shell->env);
		return (0);
	}
	i = 1;
	// すべての引数をループ処理
	while (cmd->str[i])
	{
		// 各引数を処理
		process_export_arg(shell, cmd->str[i]);
		// 引数からキーを抽出して、有効性を再チェック
		key_check = get_key(cmd->str[i]);
		if (!key_check)
			key_check = ft_strdup(cmd->str[i]);
		if (!is_valid_key(key_check))
			// 1つでも無効なキーがあれば、終了ステータスを1にする
			status = 1;
		free(key_check);
		i++;
	}
	return (status);
}
