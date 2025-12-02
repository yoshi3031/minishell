/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:47:44 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/02 10:32:33 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// lexer: 入力文字列を構文的な「トークン」に分解する関数
// 入力: コマンドライン文字列（例: "echo 'hello' | grep a > out.txt"）
// 出力: トークン構造体のリスト（単語、演算子、クォート文字列など）

t_lexer	*lexer(const char *input)
{
	t_lexer	*tokens;

	tokens = NULL;
	while (*input)
	{
		skip_spaces(&input);
		if (*input == '\0')
			break ;
		else if (is_quote(*input))
		{
			if (!read_quoted(&input, &tokens))
				return (NULL);
		}
		else if (is_operator_start(*input))
			read_operator(&input, &tokens);
		else if (*input == '$')
			read_dollar(&input, &tokens);
		else if (is_word_char(*input))
			read_word(&input, &tokens);
		else
			return (lexer_error("invalid character"));
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

// char	*ft_strdup(const char *s1)
// {
// 	char	*str;
// 	size_t	len;
// 	size_t	i;

// 	len = 0;
// 	i = 0;
// 	while (s1[len] != '\0')
// 		len++;
// 	str = (char *)malloc((len + 1) * sizeof(char));
// 	if (!str)
// 		return (NULL);
// 	while (i < len)
// 	{
// 		str[i] = s1[i];
// 		i++;
// 	}
// 	str[len] = '\0';
// 	return (str);
// }

t_lexer	*lexer_error(const char *msg)
{
	write(2, "lexer error: ", 14);
	while (*msg)
		write(2, msg++, 1);
	write(2, "\n", 1);
	return (NULL);
}
