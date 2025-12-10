/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:22:13 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 05:02:55 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "minishell.h"

//main
int	read_heredoc(const char *delimiter, bool expand, char **env, int status);
//expand
char	*expand_heredoc_line(char *line, char **env, int status);
//utils
int	abort_heredoc(char *line, int pipe_fd[2]);
void	setup_heredoc_signals(void);
int	heredoc_signal_check(void);
void	check_heredoc_eof(char *line);


#endif
