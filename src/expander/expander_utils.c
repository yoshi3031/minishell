/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 09:03:54 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:10:48 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 展開処理で使用するt_expand構造体を初期化する
// @param e: 初期化するt_expand構造体へのポインタ
// @param input: 展開対象の入力文字列
void	init_expand(t_expand *e, const char *input)
{
	e->i = 0;
	e->j = 0;
	e->in_single_quote = false;
	e->in_double_quote = false;
	e->input = input;
	e->result = NULL;
}

// ドル記号($)の処理を行う
// `$?` (終了ステータス)、`$VAR` (環境変数)、またはただの`$`文字を判別して処理する
// @param e: 展開処理の状態を保持する構造体
// @param env: 環境変数
// @param last_status: 直前のコマンドの終了ステータス
void	handle_dollar(t_expand *e, char **env, int last_status)
{
	char	var[MAX_VAR_NAME];
	size_t	k;

	e->i++;
	// `$?` (終了ステータス) の場合
	if (e->input[e->i] == '?')
	{
		handle_exit_status(e, last_status);
		e->i++;
	}
	// `$VAR` (環境変数) の場合。変数の先頭は英字かアンダースコア
	else if (ft_isalpha(e->input[e->i]) || e->input[e->i] == '_')
	{
		k = 0;
		// 変数名として有効な文字（英数字とアンダースコア）が続く限り読み取る
		while (ft_isalnum(e->input[e->i]) || e->input[e->i] == '_')
			var[k++] = e->input[e->i++];
		var[k] = '\0';
		// 読み取った変数名で環境変数の値を取得し、結果文字列にコピー
		copy_env_value(e, env, var);
	}
	// `$`の直後が変数として無効な文字の場合 (例: `$ ` や `$/`)
	// `$`をそのまま文字として扱う
	else
		e->result[e->j++] = '$';
}

// 環境変数の値を取得し、展開結果の文字列にコピーする
// @param e: 展開処理の状態を保持する構造体
// @param env: 環境変数
// @param key: 取得する環境変数のキー
void	copy_env_value(t_expand *e, char **env, const char *key)
{
	char	*val;

	// `get_env_value`は値が見つからない場合、空文字列(`""`)を返す
	val = get_env_value(env, key);
	if (val)
	{
		// 取得した値を結果文字列にコピーし、書き込んだバイト数だけインデックス`j`を進める
		e->j += ft_strlcpy(e->result + e->j, val, ft_strlen(val) + 1);
		free(val);
	}
}

// 終了ステータス `$?` を処理する
// 数値を文字列に変換し、展開結果の文字列にコピーする
// @param e: 展開処理の状態を保持する構造体
// @param last_status: 直前のコマンドの終了ステータス
void	handle_exit_status(t_expand *e, int last_status)
{
	char	*num;

	// 終了ステータス（数値）を文字列に変換
	num = ft_itoa(last_status);
	if (!num)
		exit(EXIT_FAILURE);
	// 変換した文字列を結果文字列にコピー
	e->j += ft_strlcpy(e->result + e->j, num, ft_strlen(num) + 1);
	free(num);
}

// 指定されたキーに対応する環境変数の値を取得する
// 変数が存在しない場合は空文字列を返す (bashの挙動に準拠)
// @param env: 環境変数配列
// @param key: 検索するキー
// @return: 環境変数の値の新しい文字列。常にメモリが確保されるため、呼び出し側で解放が必要
char	*get_env_value(char **env, const char *key)
{
	int		i;
	size_t	len;

	if (!key || !env)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		// `key=` の形式で前方一致するかどうかをチェック
		if (ft_strncmp(env[i], key, len) == 0 && env[i][len] == '=')
			// 一致すれば、`=`の次からの値を複製して返す
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	// 見つからなかった場合は空文字列を複製して返す
	return (ft_strdup(""));
}
