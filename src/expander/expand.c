/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 12:15:57 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 14:14:03 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_cmd(t_cmd *cmd, char **env, int last_status)
{
	expand_all_tokens(cmd->str, env, last_status);
	expand_all_redirs(cmd->redirections, env, last_status);
}

static int	update_token_list(char **args, int i, int j, char *expanded)
{
	char	*original;

	original = args[i];
	if (expanded[0] == '\0' && !has_quote(original))
	{
		free(expanded);
		free(original);
		return (0);
	}
	free(original);
	args[j] = expanded;
	return (1);
}

void	expand_all_tokens(char **args, char **env, int last_status)
{
	int		i;
	int		j;
	char	*expanded;

	i = 0;
	j = 0;
	while (args && args[i])
	{
		expanded = expand_token(args[i], env, last_status);
		j += update_token_list(args, i, j, expanded);
		i++;
	}
	if (args)
		args[j] = NULL;
}
