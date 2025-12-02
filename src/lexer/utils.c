/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:19:47 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/02 10:32:47 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_operator_start(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_word_char(char c)
{
	return (!is_separator(c) && !is_quote(c) && !is_operator_start(c));
}

void	skip_spaces(const char **input)
{
	while (**input == ' ' || **input == '\t')
		(*input)++;
}
