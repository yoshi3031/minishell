/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 12:15:57 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 12:19:11 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_cmd(t_cmd *cmd, char **env, int last_status)
{
	expand_all_tokens(cmd->str, env, last_status);
	expand_all_redirs(cmd->redirections, env, last_status);
}
