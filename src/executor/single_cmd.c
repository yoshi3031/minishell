/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 15:59:01 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 15:59:02 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	restore_std_fds(int saved_stdin, int saved_stdout)
{
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

static void	wait_single_process(t_shell *shell, pid_t pid)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFEXITED(status))
		shell->error_num = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->error_num = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit: (core dumped)\n", 2);
	}
}

static void	exec_parent_builtin(t_cmd *cmd, t_shell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (handle_redirections(cmd) == EXIT_FAILURE)
	{
		shell->error_num = 1;
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	shell->error_num = cmd->builtin(shell, cmd);
	restore_std_fds(saved_stdin, saved_stdout);
}

void	single_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->builtin == minishell_cd || cmd->builtin == minishell_exit
		|| cmd->builtin == minishell_export || cmd->builtin == minishell_unset)
	{
		exec_parent_builtin(cmd, shell);
		return ;
	}
	pid = fork();
	if (pid < 0)
		ft_error(3);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exec_cmd(shell->cmd, shell);
	}
	wait_single_process(shell, pid);
}
