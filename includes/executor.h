/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 21:01:12 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/06 15:17:37 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H
# include "minishell.h"
# include "parser.h"

int		init_paths_from_env(t_shell *shell);
char	*find_path(char **env);

void	find_cmd(t_cmd *cmd, t_shell *shell);
void	exec_cmd(t_cmd *cmd, t_shell *shell);

void	dup_cmd(t_cmd *cmd, t_shell *shell, int pipe_fd[2], int input_fd);
void	single_cmd(t_cmd *cmd, t_shell *shell);
int		multiple_cmds(t_shell *shell);

int		executor(t_shell *shell);

int		handle_redirections(t_cmd *cmd);
#endif
