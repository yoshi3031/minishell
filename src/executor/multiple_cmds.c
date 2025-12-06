/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 08:48:31 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/06 15:05:31 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/types.h>
#include <sys/wait.h>

int	ft_fork(t_shell *shell, int pipe_fd[2], int input_fd, t_cmd *cmd)
{
	static int	i;

	if (shell->reset == true)
	{
		i = 0;
		shell->reset = false;
	}
	shell->pid[i] = fork();
	if (shell->pid[i] < 0)
		ft_error(5);
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
	if (cmd->prev && dup2(input_fd, STDIN_FILENO) < 0)
		ft_error(4);
	close(pipe_fd[0]);
	if (cmd->next && dup2(pipe_fd[1], STDOUT_FILENO) < 0)
		ft_error(4);
	close(pipe_fd[1]);
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
			printed_nl = 1;
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
	while (shell->cmd)
	{
		if (shell->cmd->next)
		{
			if (pipe(pipe_fd) < 0)
				return (ft_error(5));
		}
		ft_fork(shell, pipe_fd, input_fd, shell->cmd);
		if (shell->cmd->next)
			close(pipe_fd[1]);
		if (shell->cmd->prev)
			close(input_fd);
		if (shell->cmd->next)
			input_fd = pipe_fd[0];
		if (shell->cmd->next)
			shell->cmd = shell->cmd->next;
		else
			break ;
	}
	signal(SIGINT, SIG_IGN);
	shell->error_num = wait_all_children(shell->pid, shell->pipes + 1);
	init_signals();
	shell->cmd = get_cmdlist_first(shell->cmd);
	return (0);
}
