/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_quoted.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:53:20 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/05 12:59:05 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_quoted(const char **input, t_lexer **tokens)
{
	char		quote;
	const char	*start;
	char		*quoted;
	t_tokens	type;

	start = *input;
	quote = **input;
	(*input)++;
	while (**input && **input != quote)
		(*input)++;
	if (**input != quote)
	{
		lexer_error("unclosed quote");
		return (0);
	}
	(*input)++;
	quoted = ft_strndup(start, *input - start);
	if (quote == '\'')
		type = SQUOTE_WORD;
	else
		type = DQUOTE_WORD;
	append_token(tokens, new_token(quoted, type));
	free(quoted);
	return (1);
}
