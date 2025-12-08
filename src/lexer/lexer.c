/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:47:44 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/04 15:31:20 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// トークンを抽出し、種類を判別して対応する関数を呼び出す
// クォート、演算子、ドル記号、単語文字のいずれかに基づいてトークンを読み取る
// @param input: 入力文字列へのポインタ
// @param tokens: トークンリストへのポインタ
// @return: 成功した場合は1、無効な文字が見つかった場合は0
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

// 次のトークンが現在のトークンに結合されるべきかどうかを設定する
// 現在のトークンの直後にスペースや演算子がない場合に `join_next` フラグを立てる
// @param tokens: トークンリスト
// @param input: 現在の入力位置
static void	set_join_next(t_lexer *tokens, const char *input)
{
	t_lexer	*curr;

	curr = tokens;
	while (curr && curr->next)
		curr = curr->next;
	if (curr)
	{
		if (*input && (*input != ' ' && *input != '\t' && *input != '\n')
			&& !is_operator_start(*input))
			curr->join_next = 1;
		else
			curr->join_next = 0;
	}
}

// 入力文字列をトークンのリストに変換する
// @param input: トークン化する入力文字列
// @return: トークンのリスト
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

// トークンリストの末尾に新しいトークンを追加する
// @param list: トークンリストへのポインタ
// @param new: 追加する新しいトークン
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

// 新しいトークンを作成し、初期化する
// @param str: トークンの文字列（NULLでも可）
// @param type: トークンの種類
// @return: 新しく作成されたトークン
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
	tok->join_next = 0;
	tok->heredoc_fd = -1;
	tok->next = NULL;
	tok->prev = NULL;
	return (tok);
}
