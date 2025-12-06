/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:43:06 by nhara             #+#    #+#             */
/*   Updated: 2025/12/06 14:52:30 by yotakagi         ###   ########.fr       */
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
	shell->reset = false;
	shell->pid = NULL;
	shell->heredoc = false;
	shell->error_num = 0;
	if (init_paths_from_env(shell) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
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
	if (shell->lexer_list)
		free_lexer(&shell->lexer_list);
	if (init_paths_from_env(shell) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	shell->cmd = NULL;
	shell->args = NULL;
	shell->lexer_list = NULL;
	shell->pid = NULL;
	shell->heredoc = false;
	shell->reset = true;
	return (EXIT_SUCCESS);
}

int	loop(t_shell *shell)
{
	char	*tmp;

	shell->args = readline("minishell> ");
	if (g_signal)
	{
		shell->error_num = 130;
		g_signal = 0;
	}
	if (!shell->args)
	{
		if (isatty(STDIN_FILENO))
			ft_putendl_fd("exit", STDERR_FILENO);
		exit(EXIT_SUCCESS);
	}
	tmp = ft_strtrim(shell->args, " \t");
	free(shell->args);
	shell->args = tmp;
	if (shell->args[0] == '\0')
		return (shell->error_num);
	add_history(shell->args);
	if (!validate_quotes(shell->args))
		return (ft_error(2));
	shell->lexer_list = lexer(shell->args);
	if (!shell->lexer_list)
		return (ft_error(1));
	if (parser(shell) == EXIT_FAILURE)
	{
		return (EXIT_FAILURE);
	}
	executor(shell);
	return (shell->error_num);
}
