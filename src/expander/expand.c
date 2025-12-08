/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 12:15:57 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 14:14:03 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// コマンドの引数とリダイレクションの両方を展開する
// @param cmd: コマンド構造体
// @param env: 環境変数
// @param last_status: 直前のコマンドの終了ステータス
void	expand_cmd(t_cmd *cmd, char **env, int last_status)
{
	expand_all_tokens(cmd->str, env, last_status);
	expand_all_redirs(cmd->redirections, env, last_status);
}

// 展開後のトークンでトークンリストを更新する
// 展開結果が空文字列で、かつ元のトークンがクォートされていなかった場合、
// トークンをリストから削除する (戻り値0により)
// @param args: 引数の配列
// @param i: 元のトークンのインデックス
// @param j: 更新後のトークンのインデックス
// @param expanded: 展開後の文字列
// @return: トークンを維持する場合は1、削除する場合は0
static int	update_token_list(char **args, int i, int j, char *expanded)
{
	char	*original;

	original = args[i];
	if (expanded[0] == '\0' && !has_quote(original))
	{
		free(expanded);
		free(original);
		return (0);
	}
	free(original);
	args[j] = expanded;
	return (1);
}

// 文字列配列（コマンドの引数など）のすべてのトークンを展開する
// @param args: 展開するトークンの配列
// @param env: 環境変数
// @param last_status: 直前のコマンドの終了ステータス
void	expand_all_tokens(char **args, char **env, int last_status)
{
	int		i;
	int		j;
	char	*expanded;

	i = 0;
	j = 0;
	while (args && args[i])
	{
		expanded = expand_token(args[i], env, last_status);
		j += update_token_list(args, i, j, expanded);
		i++;
	}
	if (args)
		args[j] = NULL;
}
