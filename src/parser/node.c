/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_lexer_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhara <nhara@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:53:20 by nhara             #+#    #+#             */
/*   Updated: 2025/05/22 10:01:23 by nhara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// +---+      +---+      +---+
// | A | <--> | B | <--> | C |
// +---+      +---+      +---+

// 指定したノードを削除
t_lexer *remove_one_node(t_lexer **lst)
{
	// 文字列が存在する場合
	if ((*lst)->str)
	{
		free((*lst)->str);
		(*lst)->str = NULL;
	}
	free(*lst);
	*lst = NULL;
	return (NULL);
}

// 最初のノードを削除
void remove_first_node(t_lexer **lst)
{
	t_lexer *node;

	node = *lst;
	*lst = node->next;		// リストの先頭をBに設定
	remove_one_node(&node); // 先頭を削除
	// BはまだAを指しているため、NULLを設定
	if (*lst)
		(*lst)->prev = NULL;
}

void remove_node(t_lexer **lst, int target)
{
	t_lexer *node;	// 削除対象を探す
	t_lexer *prev;	// nodeの1つ前のノードを保存
	t_lexer *start; // リストの先頭を保存
	// 初期化
	start = *lst;
	node = start;
	// 先頭のノードが削除対象の場合
	if ((*lst)->i == target)
	{
		remove_first_node(lst);
		return;
	}
	// リストを巡回し、i == key を持つノードを探す
	while (node && node->i != target)
	{
		prev = node;
		node = node->next;
	}
	// この時点で、見つからない場合はnode == NULL、見つかる場合はnode == 削除対象になる
	// 見つかった：削除するノード（B）をスキップし、前（A）と後ろ（C）のノードを接続
	if (node)
		prev->next = node->next;
	// 見つからない：最後のノードに設定
	else
		prev->next = NULL;
	// 現時点でCはBを指しているため、Aを指すようする。
	//  prev->nextはC。そのprevはB。それをAに設定。
	if (prev->next)
		prev->next->prev = prev;
	// Bを削除
	remove_one_node(&node);
	*lst = start;
}

// 新しいトークン(ノード)を作る
// 毎回indexを０に代入していたため削除
t_lexer *create_node(char *str, int token)
{
	t_lexer *new_node;

	new_node = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_node)
		return (0);
	new_node->str = str;
	new_node->token = token;
	new_node->i = -1;
	new_node->heredoc_fd = -1;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

//node追加時にindex割り振りに変更
void add_node_back(t_lexer **lst, t_lexer *new_node)
{
	t_lexer *tmp;

	tmp = *lst;
	if (*lst == NULL)
	{
		*lst = new_node;
		new_node->i = 0;
		return;
	}
	// 最後まで進める
	while (tmp->next != NULL)
		tmp = tmp->next;
	// ノードを後ろに追加
	tmp->next = new_node;
	// 追加したノードが前にtmpを指すようにする
	new_node->prev = tmp;
	new_node->i = tmp->i + 1;
}
