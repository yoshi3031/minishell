/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:47:44 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/09 16:03:36 by yotakagi         ###   ########.fr       */
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
	// 現在の文字がクォート(' or ")かチェック
	if (is_quote(**input))
	{
		// クォートで囲まれた部分を読み取る。閉じられていない場合はエラー
		if (!read_quoted(input, tokens))
			return (0);
	}
	// 現在の文字が演算子(<, >, |)の開始文字かチェック
	else if (is_operator_start(**input))
		read_operator(input, tokens);
	// 現在の文字がドル記号($)かチェック
	else if (**input == '$')
		read_dollar(input, tokens);
	// 上記のいずれでもないが、単語を構成する文字かチェック
	else if (is_word_char(**input))
		read_word(input, tokens);
	else
	{
		// いずれの条件にも当てはまらない無効な文字の場合、エラーを報告
		lexer_error("invalid character");
		return (0);
	}
	// トークン抽出成功
	return (1);
}

// 次のトークンが現在のトークンに結合されるべきかどうかを設定する
// 現在のトークンの直後にスペースや演算子がない場合に `join_next` フラグを立てる
// @param tokens: トークンリスト
// @param input: 現在の入力位置
static void	set_join_next(t_lexer *tokens, const char *input)
{
	t_lexer	*curr;

	// トークンリストの末尾のトークンを探す
	curr = tokens;
	while (curr && curr->next)
		curr = curr->next;
	// 末尾のトークンが存在する場合
	if (curr)
	{
		// 次の文字が存在し、それが空白や演算子の開始文字でなければ、
		// 現在のトークンと次のトークンは結合されるべき（例: echo"hello"）
		if (*input && (*input != ' ' && *input != '\t' && *input != '\n')
			&& !is_operator_start(*input))
			curr->join_next = 1;
		else
			// それ以外の場合は結合しない
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
	// 入力文字列の終端に達するまでループ
	while (*input)
	{
		// 先頭の空白文字をスキップ
		skip_spaces(&input);
		// 空白をスキップした後、文字列の終端に達したらループを抜ける
		if (*input == '\0')
			break ;
		// トークンを1つ抽出し、失敗したらNULLを返して終了
		if (!extract_token(&input, &tokens))
			return (NULL);
		// 抽出したトークンの直後の文字を見て、次のトークンと結合すべきか判断
		set_join_next(tokens, input);
	}
	// すべてのトークンを抽出した後、入力の終わりを示すトークンを追加
	append_token(&tokens, new_token(NULL, END_OF_INPUT));
	return (tokens);
}

// トークンリストの末尾に新しいトークンを追加する
// @param list: トークンリストへのポインタ
// @param new: 追加する新しいトークン
void	append_token(t_lexer **list, t_lexer *new)
{
	t_lexer	*last;

	// リストが空の場合、新しいトークンをリストの先頭にする
	if (!*list)
	{
		*list = new;
		return ;
	}
	// リストの最後のノードを見つける
	last = *list;
	while (last->next)
		last = last->next;
	// 新しいトークンをリストの末尾に追加し、双方向リストのリンクを設定
	last->next = new;
	new->prev = last;
	// トークンのインデックスを更新
	new->i = last->i + 1;
}

// 新しいトークンを作成し、初期化する
// @param str: トークンの文字列（NULLでも可）
// @param type: トークンの種類
// @return: 新しく作成されたトークン
t_lexer	*new_token(char *str, t_tokens type)
{
	t_lexer	*tok;

	// トークン構造体のメモリを確保
	tok = malloc(sizeof(t_lexer));
	if (!tok)
		return (NULL);
	// 文字列が提供されていれば、それを複製して格納
	if (str)
		tok->str = ft_strdup(str);
	else
		tok->str = NULL;
	// トークンの各メンバを初期値で設定
	tok->token = type;
	tok->i = 0;
	tok->join_next = 0;
	tok->heredoc_fd = -1;
	tok->next = NULL;
	tok->prev = NULL;
	return (tok);
}
