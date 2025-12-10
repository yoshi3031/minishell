/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 15:28:38 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/10 13:36:35 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2d_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	count_2d_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr && arr[i])
		i++;
	return (i);
}

void	export_error(const char *s)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd((char *)s, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

char	*get_key(const char *arg)
{
	size_t	len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	return (ft_substr(arg, 0, len));
}

char	*get_val(const char *arg)
{
	char	*val;

	val = ft_strchr(arg, '=');
	if (!val)
		return (NULL);
	return (ft_strdup(val + 1));
}
