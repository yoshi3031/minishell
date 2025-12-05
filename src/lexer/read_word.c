/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:46:25 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/05 12:58:43 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_word(const char **input, t_lexer **tokens)
{
	const char	*start = *input;
	char		*word;

	while (**input && !is_separator(**input))
		(*input)++;
	word = ft_strndup(start, *input - start);
	append_token(tokens, new_token(word, WORD));
	free(word);
}

int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<'
		|| c == '>' || c == '\'' || c == '"' || c == '\0');
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*res;
	size_t	i;

	res = malloc(n + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[n] = '\0';
	return (res);
}

// void	print_tokens(t_lexer *tokens)
//{
//	while (tokens)
//	{
//		printf("token[%d] type=%d str=\"%s\"\n", tokens->i, tokens->token,
//			tokens->str ? tokens->str : "(null)");
//		tokens = tokens->next;
//	}
//}

// int	main(void)
//{
//	const char	*line = "echo hello";
//	t_lexer		*tokens;

//	tokens = lexer(line);
//	print_tokens(tokens); // 確認用
//	return (0);
//}
