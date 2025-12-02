/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_dollar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:16:53 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/02 10:32:37 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_dollar(const char **input, t_lexer **tokens)
{
	const char	*start;
	char		*var;

	start = *input;
	(*input)++;
	if (**input == '?')
	{
		(*input)++;
		append_token(tokens, new_token(ft_strdup("$?"), EXIT_STATUS));
		return ;
	}
	while (**input && (ft_isalnum(**input) || **input == '_'))
		(*input)++;
	if (*input == start)
	{
		append_token(tokens, new_token(ft_strdup("$"), WORD));
		return ;
	}
	var = ft_strndup(start, *input - start);
	append_token(tokens, new_token(var, ENV_VAR));
}
