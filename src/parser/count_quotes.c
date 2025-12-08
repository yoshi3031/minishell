/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 18:14:02 by nhara             #+#    #+#             */
/*   Updated: 2025/12/08 14:57:10 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//remove comments
#include "minishell.h"

int	find_pair_quote(char *args, int i, int *num_quotes, int quote)
{
	int	j;

	j = i + 1;
	*num_quotes += 1;
	while (args[j] && args[j] != quote)
		j++;
	if (args[j] == quote)
		*num_quotes += 1;
	return (j - i);
}

int	validate_quotes(char *args)
{
	int	i;
	int	d_quotes;
	int	s_quotes;

	i = -1;
	d_quotes = 0;
	s_quotes = 0;
	while (args[++i])
	{
		if (args[i] == '"')
			i += find_pair_quote(args, i, &d_quotes, '"');
		if (args[i] == '\'')
			i += find_pair_quote(args, i, &s_quotes, '\'');
	}
	if ((s_quotes % 2 != 0) || (d_quotes % 2 != 0))
		return (0);
	return (1);
}
