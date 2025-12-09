/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 13:46:25 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:03:38 by yotakagi         ###   ########.fr       */
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

	// 単語の区切り文字（セパレータ）に遭遇するか、文字列の終端に達するまでポインタを進める
	while (**input && !is_separator(**input))
		(*input)++;
	// 単語の開始位置から現在の位置までの部分文字列を複製
	word = ft_strndup(start, *input - start);
	// 複製した文字列でWORDトークンを作成し、リストに追加
	append_token(tokens, new_token(word, WORD));
	free(word);
}

// 文字が単語の区切り文字（セパレータ）であるかどうかを判定する
// @param c: 判定する文字
// @return: 区切り文字であれば1、そうでなければ0
int	is_separator(char c)
{
	// スペース、タブ、改行、パイプ、リダイレクト、クォート、文字列終端を区切り文字とみなす
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

	// 複製する文字数 + 終端ナル文字('\0')分のメモリを確保
	res = malloc(n + 1);
	if (!res)
		return (NULL);
	i = 0;
	// 指定された文字数(n)だけ文字をコピー
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	// 文字列の末尾に終端ナル文字を追加
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
