/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 09:35:03 by nhara             #+#    #+#             */
/*   Updated: 2025/12/03 12:43:34 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_builtin_fn	builtin_arr(char *str)
{
	if (!str)
		return (NULL);
	if (!ft_strcmp(str, "echo"))
		return (minishell_echo);
	if (!ft_strcmp(str, "cd"))
		return (minishell_cd);
	if (!ft_strcmp(str, "pwd"))
		return (minishell_pwd);
	if (!ft_strcmp(str, "export"))
		return (minishell_export);
	if (!ft_strcmp(str, "unset"))
		return (minishell_unset);
	if (!ft_strcmp(str, "env"))
		return (minishell_env);
	if (!ft_strcmp(str, "exit"))
		return (minishell_exit);
	return (NULL);
}
