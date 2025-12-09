/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:13:28 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:26:38 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// コマンドリスト内のすべてのコマンドを展開する
// @param cmds: コマンドリストの先頭
// @param env: 環境変数
// @param last_status: 直前のコマンドの終了ステータス
void	expand_all_cmds(t_cmd *cmds, char **env, int last_status)
{
	t_cmd	*cur;

	cur = cmds;
	// コマンドリストを先頭から末尾まで走査
	while (cur)
	{
		// 各コマンド構造体に対して変数展開を実行
		expand_cmd(cur, env, last_status);
		cur = cur->next;
	}
}

// リダイレクションリスト内のすべてのファイル名を展開する
// ヒアドキュメント(`HEREDOC`)は展開の対象外
// @param redir: リダイレクションリストの先頭
// @param env: 環境変数
// @param last_status: 直前のコマンドの終了ステータス
void	expand_all_redirs(t_lexer *redir, char **env, int last_status)
{
	char	*expanded;

	// リダイレクションリストを走査
	while (redir)
	{
		// ヒアドキュメントのデリミタは展開の対象外
		if (redir->token == HEREDOC)
		{
			redir = redir->next;
			continue ;
		}
		// リダイレクションのファイル名（`redir->str`）を展開
		expanded = expand_token(redir->str, env, last_status);
		// 元の文字列を解放
		if (redir->str)
			free(redir->str);
		// 展開後の文字列にポインタを更新
		redir->str = expanded;
		redir = redir->next;
	}
}

// 文字列にクォート（シングルまたはダブル）が含まれているかチェックする
// @param str: チェックする文字列
// @return: クォートが含まれていればtrue、なければfalse
bool	has_quote(const char *str)
{
	// 文字列の終端までチェック
	while (*str)
	{
		// シングルクォートまたはダブルクォートが見つかればtrueを返す
		if (*str == '\'' || *str == '\"')
			return (true);
		str++;
	}
	return (false);
}

// 展開後の文字列を構築するメインロジック
// クォートの状態を管理し、シングルクォート内では展開を行わない
// '$'を検知すると`handle_dollar`を呼び出して変数を展開する
// @param e: 展開処理の状態を保持する構造体
// @param env: 環境変数
// @param last_status: 直前のコマンドの終了ステータス
static void	fill_expanded_str(t_expand *e, char **env, int last_status)
{
	// 入力文字列の終端までループ
	while (e->input[e->i])
	{
		// ダブルクォート内ではないシングルクォートを処理
		if (e->input[e->i] == '\'' && !e->in_double_quote)
		{
			e->in_single_quote = !e->in_single_quote;
			e->i++;
		}
		// シングルクォート内ではないダブルクォートを処理
		else if (e->input[e->i] == '"' && !e->in_single_quote)
		{
			e->in_double_quote = !e->in_double_quote;
			e->i++;
		}
		// シングルクォート内ではないドル記号を処理（変数展開）
		else if (e->input[e->i] == '$' && !e->in_single_quote)
			handle_dollar(e, env, last_status);
		else
			// 上記以外は通常の文字として結果文字列にコピー
			e->result[e->j++] = e->input[e->i++];
	}
	// 結果文字列をNULLで終端
	e->result[e->j] = '\0';
}

// 単一のトークン（文字列）を展開する
// 1. 展開後の長さを計算 (`calc_expanded_length`)
// 2. 結果を格納するためのメモリを確保
// 3. `fill_expanded_str`を呼び出して実際に文字列を構築
// @param input: 展開する入力文字列
// @param env: 環境変数
// @param last_status: 直前のコマンドの終了ステータス
// @return: 展開後の新しい文字列
char	*expand_token(const char *input, char **env, int last_status)
{
	t_expand	e;
	size_t		len;

	if (!input)
		return (NULL);
	// 展開処理用の構造体を初期化
	init_expand(&e, input);
	// 展開後の最終的な文字列長を計算
	len = calc_expanded_length(input, env, last_status);
	// 計算した長さ分のメモリを確保
	e.result = malloc(len + 1);
	if (!e.result)
		return (NULL);
	// 実際に展開を行い、確保したメモリに結果を書き込む
	fill_expanded_str(&e, env, last_status);
	return (e.result);
}
