/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 14:22:13 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 15:18:07 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_H
# define HEREDOC_H

# include "minishell.h"

int	read_heredoc(const char *delimiter, bool expand, char **env, int status);

#endif
