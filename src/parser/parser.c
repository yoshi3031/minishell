/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 19:01:20 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/05 12:37:48 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int parser(t_shell *shell) // TODO
{
	t_cmd *node; // コマンドを保持する構造体
	t_parser_shell parser_shell;

	shell->cmd = NULL;
	// パイプの数を数え、パイプ関連のエラーを検出
	count_pipes(shell->lexer_list, shell);
	// エラー：最初にパイプ
	if (shell->lexer_list->token == PIPE)
		return (parser_double_token_error(shell->lexer_list->token));
	// lexerの最後まで繰り返す
	while (shell->lexer_list && shell->lexer_list->token != END_OF_INPUT)
	{

		// パイプの削除
		if (shell->lexer_list && shell->lexer_list->token == PIPE)
			remove_node(&shell->lexer_list, shell->lexer_list->i);
		// パイプエラーの確認
		if (pipe_errors(shell, shell->lexer_list->token))
			return (EXIT_FAILURE);
		// コマンド解析用の構造体を初期化
		parser_shell = init_parser_shell(shell->lexer_list, shell);
		// 新しいコマンドを構築
		node = init_cmd(&parser_shell);
		if (!node)
		{
			if(g_signal)
			{
				//free_lexer?
				shell->error_num = 1;
				return (EXIT_FAILURE);
			}
			ft_error(0);
			return (EXIT_FAILURE);
		}
		// 構築したコマンドを、コマンドリストに追加
		// コマンドリストが空の場合、先頭に入れる
		if (!shell->cmd)
			shell->cmd = node;
		else
			add_cmd_back(&shell->cmd, node);
		// トークンリストを更新
		shell->lexer_list = parser_shell.lexer_list;
	}
	return (EXIT_SUCCESS);
}
