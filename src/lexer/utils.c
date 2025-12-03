/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:19:47 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 12:14:59 by yotakagi         ###   ########.fr       */
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

t_lexer	*lexer_error(const char *msg)
{
	write(2, "lexer error: ", 14);
	while (*msg)
		write(2, msg++, 1);
	write(2, "\n", 1);
	return (NULL);
}
