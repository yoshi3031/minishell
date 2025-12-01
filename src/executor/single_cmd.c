/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 16:14:16 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/01 07:23:26 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 標準入出力の復元用ヘルパー関数
static void restore_std_fds(int saved_stdin, int saved_stdout)
{
    // 退避しておいたFDを元に戻す
    if (saved_stdin != -1)
    {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
    if (saved_stdout != -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
}

void	single_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t pid;
	int status;
	int saved_stdin;
	int saved_stdout;
	
	if (cmd->builtin == minishell_cd || cmd->builtin == minishell_exit
		|| cmd->builtin == minishell_export || cmd->builtin == minishell_unset)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if(handle_redirections(cmd) == EXIT_FAILURE)
		{
			shell->error_num = 1;
			restore_std_fds(saved_stdin, saved_stdout);
			return ;
		}
		shell->error_num = cmd->builtin(shell, cmd);
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	pid = fork();
	if (pid < 0)
		ft_error(3, shell);
	if (pid == 0)
		exec_cmd(shell->cmd, shell);
	// 子の終了待ち
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->error_num = WEXITSTATUS(status);
	else if(WIFSIGNALED(status))
		shell->error_num = 128 + WTERMSIG(status);
}
