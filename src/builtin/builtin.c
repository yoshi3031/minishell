/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:35:03 by nhara             #+#    #+#             */
/*   Updated: 2025/12/02 10:31:59 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int (*builtin_arr(char *str))(t_shell *shell, t_cmd *cmd)
{
	int	i;

	static void *builtin_func[7][2] = {
		{"echo", minishell_echo},
		{"cd", minishell_cd},
		{"pwd", minishell_pwd},
		{"export", minishell_export},
		{"unset", minishell_unset},
		{"env", minishell_env},
		{"exit", minishell_exit},
	};
	i = 0;
	while (i < 7)
	{
		if (str && !ft_strcmp(builtin_func[i][0], str))
			return (builtin_func[i][1]);
		i++;
	}
	return (NULL);
}
