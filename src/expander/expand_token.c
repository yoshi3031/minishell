/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 11:13:28 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 14:09:05 by yotakagi         ###   ########.fr       */
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
	while (cur)
	{
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

	while (redir)
	{
		if (redir->token == HEREDOC)
		{
			redir = redir->next;
			continue ;
		}
		expanded = expand_token(redir->str, env, last_status);
		if (redir->str)
			free(redir->str);
		redir->str = expanded;
		redir = redir->next;
	}
}

// 文字列にクォート（シングルまたはダブル）が含まれているかチェックする
// @param str: チェックする文字列
// @return: クォートが含まれていればtrue、なければfalse
bool	has_quote(const char *str)
{
	while (*str)
	{
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
	while (e->input[e->i])
	{
		if (e->input[e->i] == '\'' && !e->in_double_quote)
		{
			e->in_single_quote = !e->in_single_quote;
			e->i++;
		}
		else if (e->input[e->i] == '"' && !e->in_single_quote)
		{
			e->in_double_quote = !e->in_double_quote;
			e->i++;
		}
		else if (e->input[e->i] == '$' && !e->in_single_quote)
			handle_dollar(e, env, last_status);
		else
			e->result[e->j++] = e->input[e->i++];
	}
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
	init_expand(&e, input);
	len = calc_expanded_length(input, env, last_status);
	e.result = malloc(len + 1);
	if (!e.result)
		return (NULL);
	fill_expanded_str(&e, env, last_status);
	return (e.result);
}
