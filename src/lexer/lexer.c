/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:47:44 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 13:24:57 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	extract_token(const char **input, t_lexer **tokens)
{
	if (is_quote(**input))
	{
		if (!read_quoted(input, tokens))
			return (0);
	}
	else if (is_operator_start(**input))
		read_operator(input, tokens);
	else if (**input == '$')
		read_dollar(input, tokens);
	else if (is_word_char(**input))
		read_word(input, tokens);
	else
	{
		lexer_error("invalid character");
		return (0);
	}
	return (1);
}

static void	set_join_next(t_lexer *tokens, const char *input)
{
	t_lexer	*curr;

	curr = tokens;
	while (curr && curr->next)
		curr = curr->next;
	if (curr)
	{
		if (*input && !is_separator(*input) && !is_operator_start(*input))
			curr->join_next = 1;
		else
			curr->join_next = 0;
	}
}

t_lexer	*lexer(const char *input)
{
	t_lexer	*tokens;

	tokens = NULL;
	while (*input)
	{
		skip_spaces(&input);
		if (*input == '\0')
			break ;
		if (!extract_token(&input, &tokens))
			return (NULL);
		set_join_next(tokens, input);
	}
	append_token(&tokens, new_token(NULL, END_OF_INPUT));
	return (tokens);
}

void	append_token(t_lexer **list, t_lexer *new)
{
	t_lexer	*last;

	if (!*list)
	{
		*list = new;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new;
	new->prev = last;
	new->i = last->i + 1;
}

t_lexer	*new_token(char *str, t_tokens type)
{
	t_lexer	*tok;

	tok = malloc(sizeof(t_lexer));
	if (!tok)
		return (NULL);
	if (str)
		tok->str = ft_strdup(str);
	else
		tok->str = NULL;
	tok->token = type;
	tok->i = 0;
	tok->next = NULL;
	tok->prev = NULL;
	return (tok);
}
