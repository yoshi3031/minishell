/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:13:27 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 14:56:54 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

char	**ft_arrdup(char **arr)
{
	char	**result;
	size_t	i;

	i = 0;
	while (arr[i] != NULL)
		i++;
	result = ft_calloc(sizeof(char *), i + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
	{
		result[i] = ft_strdup(arr[i]);
		if (result[i] == NULL)
		{
			free_arr(result);
			return (NULL);
		}
		i++;
	}
	return (result);
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)av;
	if (ac != 1)
	{
		ft_putstr_fd("minishell: too many arguments\n", 2);
		return (1);
	}
	shell.env = ft_arrdup(env);
	init_shell(&shell);
	while (1)
	{
		shell.error_num = loop(&shell);
		if (reset_shell(&shell) == EXIT_FAILURE)
		{
			ft_putstr_fd("minishell: fatal error in reset_shell\n", 2);
			break ;
		}
	}
	free_arr(shell.env);
	return (shell.error_num);
}
