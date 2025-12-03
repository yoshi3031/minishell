/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:43:06 by nhara             #+#    #+#             */
/*   Updated: 2025/12/03 12:28:06 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int loop(t_shell *shell); //TODO　これ、ここに必要？

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
// シェルの初期化
void	init_shell(t_shell *shell)
{
	shell->cmd = NULL;
	shell->args = NULL;
	shell->lexer_list = NULL;
	shell->reset = false; // shellのリセットが不要であることを示す
	shell->pid = NULL;
	shell->heredoc = false;
	// shell->error_num = 0;
	// shell->stop_heredoc = 0;
	// shell->in_cmd = 0;
	// shell->in_heredoc = 0;
	if (init_paths_from_env(shell) == EXIT_FAILURE)
	{
		printf("msg"); //エラーメッセージどうする？
		exit(EXIT_FAILURE);
	}
	init_signals();
}

// シェル全体のデータ解放＋最初期化＋次のループへ
int	reset_shell(t_shell *shell)
{
	// 各々のコマンドを解放
	if (shell->cmd)
		free_cmd(&shell->cmd);
	// コマンドを保持していた文字列を解放
	if (shell->args)
		free(shell->args);
	if (shell->pid)
		free(shell->pid);
	// パスリストを解放
	if (shell->paths)
		free_arr(shell->paths);
	// shell構造体再設定
	init_shell(shell);
	// 初期化するようにフラグを立てる
	shell->reset = true;
	loop(shell);
	return (EXIT_SUCCESS); // リセット完了を伝える,1->0に変更
}

int	loop(t_shell *shell)
{
	char	*tmp;

	shell->args = readline("minishell> ");
	tmp = ft_strtrim(shell->args, " \t"); //タブ文字も除去する変更
	free(shell->args);
	shell->args = tmp;
	if (!shell->args)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		exit(EXIT_SUCCESS);
	}
	if (shell->args[0] == '\0')
		return (reset_shell(shell));
	add_history(shell->args);
	if (!validate_quotes(shell->args))
		return (ft_error(2, shell));
	shell->lexer_list = lexer(shell->args);
	if (!shell->lexer_list)
		return (ft_error(1, shell));
	parser(shell);
	executor(shell);
	reset_shell(shell);
	return (shell->error_num);
}
