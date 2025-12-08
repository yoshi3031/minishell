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

// クォートされていない単語を読み取り、WORDトークンとして追加する
// セパレータ（スペース、演算子、クォートなど）が現れるまでを1つの単語とする
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
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

// 文字が単語の区切り文字（セパレータ）であるかどうかを判定する
// @param c: 判定する文字
// @return: 区切り文字であれば1、そうでなければ0
int	is_separator(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '|' || c == '<'
		|| c == '>' || c == '\'' || c == '"' || c == '\0');
}

// 文字列の先頭からn文字を複製する（strndupの自作版）
// @param s: 複製元の文字列
// @param n: 複製する文字数
// @return: 新しく確保された文字列
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
