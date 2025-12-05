/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 08:48:31 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/05 11:46:19 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>

// イメージ
//      ┌───────────── pipe ─────────────┐
//      │                                │
// write → fd[1]                  fd[0] → read
// 書き込み                        //読み取り

//[ ls ] --pipe_fd[1]→───┐
//                       │パイプの中
//               pipe_fd[0]──→ [ grep ]

// input_fd は 「パイプの読み取り口（pipe_fd[0]）**の番号を一時的に保存している変数

int	ft_fork(t_shell *shell, int pipe_fd[2], int input_fd, t_cmd *cmd)
{
	static int	i;

	// 3コマンドあったらft_forkは3回呼ばれる。staticにすることでiを保持する
	if (shell->reset == true)
	{
		i = 0;
		shell->reset = false;
	}
	shell->pid[i] = fork();
	// fork失敗：親プロセスに-1が返り、子プロセスは生成されない
	// fork成功：親プロセスに子のPIDが返る。子プロセスに0が返る
	if (shell->pid[i] < 0)
		ft_error(3); // TODO forkが失敗した場合のエラー文
	if (shell->pid[i] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		dup_cmd(cmd, shell, pipe_fd, input_fd);
	}
	i++;
	return (EXIT_SUCCESS);
}

void	dup_cmd(t_cmd *cmd, t_shell *shell, int pipe_fd[2], int input_fd)
{
	// 今のcmdの前にcmdがある場合
	// 標準入力（STDIN_FILENO）の読み取り先を、input_fd(前のコマンドの出力)にすり替える
	// dup2が失敗すると-1が返る
	if (cmd->prev && dup2(input_fd, STDIN_FILENO) < 0)
		ft_error(4); // TODO
	// 現コマンドの読み取りはもう使わないので閉じる
	close(pipe_fd[0]);
	// 次のコマンドがあるなら、今のコマンドの標準出力を end[1]（パイプの書き込み口）に変更
	if (cmd->next && dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		ft_error(4); // TODO
	close(pipe_fd[1]);
	// dup2(input_fd, STDIN_FILENO)でinput_fdの内容はSTDIN_FILENOに複製済みだから不要
	// 前にコマンドがない場合は、そもそもinput_fdを使っていない
	if (cmd->prev)
		close(input_fd);
	exec_cmd(cmd, shell);
}

int	wait_all_children(int *pid, int cmd_count)
{
	int	i;
	int	status;
	int	last_status_code;
	int	last_pid;
	int	printed_nl;

	printed_nl = 0;
	last_status_code = 0;
	last_pid = pid[cmd_count - 1];
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pid[i], &status, 0);
		if (WTERMSIG(status) == SIGINT && !printed_nl)
		{
			write(1, "\n", 1);
			printed_nl = 1; // 一回だしたらもう出さない
		}
		else if (WTERMSIG(status) == SIGQUIT && !printed_nl)
		{
			ft_putstr_fd("Quit: (core dumped)\n", 2);
			printed_nl = 1;
		}
		if (pid[i] == last_pid)
		{
			if (WIFEXITED(status))
				last_status_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status_code = 128 + WTERMSIG(status);
		}
		i++;
	}
	return (last_status_code);
}

t_cmd	*get_cmdlist_first(t_cmd *node)
{
	if (!node)
		return (NULL);
	while (node->prev != NULL)
		node = node->prev;
	return (node);
}

int	multiple_cmds(t_shell *shell)
{
	int	pipe_fd[2];
	int	input_fd;

	input_fd = STDIN_FILENO;
	// 一つずつコマンド処理
	while (shell->cmd)
	{
		// 次にコマンドがあったらパイプ作成
		if (shell->cmd->next)
		{
			if(pipe(pipe_fd) < 0)
				return (ft_error(5));
		}
		ft_fork(shell, pipe_fd, input_fd, shell->cmd);
		close(pipe_fd[1]);
		// 2番目以降のコマンドで、前のパイプの読み取り口 fd_in を使い終わったら閉じる
		if (shell->cmd->prev)
			close(input_fd);
		if (shell->cmd->next)
			input_fd = pipe_fd[0];
		// 次のコマンドへ
		if (shell->cmd->next)
			shell->cmd = shell->cmd->next;
		else
			break ;
	}
	signal(SIGINT, SIG_IGN); //^Cを無視
	// 子プロセスを全て待つ
	shell->error_num = wait_all_children(shell->pid, shell->pipes + 1);
	init_signals();
	shell->cmd = get_cmdlist_first(shell->cmd);
	return (0);
}
