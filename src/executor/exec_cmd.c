/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:57:24 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/06 15:05:10 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_with_perror(char *cmd, char *msg, int error_num)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(error_num);
}

int	is_dir(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

void	find_cmd(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*cmd_path;

	if (!shell->paths)
		exit_with_perror(cmd->str[0], "command not found", 127);
	i = 0;
	while (shell->paths[i])
	{
		cmd_path = ft_strjoin(shell->paths[i], cmd->str[0]);
		if (!cmd_path)
		{
			perror("minishell: malloc");
			exit(EXIT_FAILURE);
		}
		if (!access(cmd_path, F_OK | X_OK))
			execve(cmd_path, cmd->str, shell->env);
		free(cmd_path);
		i++;
	}
	exit_with_perror(cmd->str[0], "command not found", 127);
}

void	exec_cmd(t_cmd *cmd, t_shell *shell)
{
	if (handle_redirections(cmd) == EXIT_FAILURE)
		exit(EXIT_FAILURE);
	if (!cmd->str || !cmd->str[0])
		exit(EXIT_SUCCESS);
	if (cmd->builtin)
		exit(cmd->builtin(shell, cmd));
	if (ft_strchr(cmd->str[0], '/'))
	{
		if (access(cmd->str[0], F_OK) == -1)
			exit_with_perror(cmd->str[0], "No such file or directory", 127);
		if (is_dir(cmd->str[0]))
			exit_with_perror(cmd->str[0], "Is a directory", 126);
		if (access(cmd->str[0], X_OK) == -1)
			exit_with_perror(cmd->str[0], "Permission denied", 126);
		execve(cmd->str[0], cmd->str, shell->env);
		perror("minishell");
		exit(EXIT_FAILURE);
	}
	else if (cmd->str[0][0] != '\0')
		find_cmd(cmd, shell);
	exit_with_perror(cmd->str[0], "command not found", 127);
}
