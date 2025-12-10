/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_operator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 15:35:34 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/10 13:38:05 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_operator(const char **input, t_lexer **tokens)
{
	if (read_two_char_op(input, tokens))
		return ;
	if (read_one_char_op(input, tokens))
		return ;
	lexer_error("unexpected operator");
}

int	read_two_char_op(const char **input, t_lexer **tokens)
{
	t_tokens	type;

	if (**input == '<' && *(*input + 1) == '<')
		type = HEREDOC;
	else if (**input == '>' && *(*input + 1) == '>')
		type = REDIR_APPEND;
	else
		return (0);
	*input += 2;
	append_token(tokens, new_token(NULL, type));
	return (1);
}

int	read_one_char_op(const char **input, t_lexer **tokens)
{
	t_tokens	type;

	if (**input == '|')
		type = PIPE;
	else if (**input == '<')
		type = REDIR_IN;
	else if (**input == '>')
		type = REDIR_OUT;
	else
		return (0);
	(*input)++;
	append_token(tokens, new_token(NULL, type));
	return (1);
}
