/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nhara <nhara@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 09:06:44 by nagisa            #+#    #+#             */
/*   Updated: 2025/05/22 10:45:01 by nhara            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredocs(t_lexer *redirections)
{
	while (redirections)
	{
		if (redirections->token == HEREDOC && redirections->heredoc_fd != -1)
		{
			close(redirections->heredoc_fd);
			redirections->heredoc_fd = -1;
		}
		redirections = redirections->next;
	}
}

// コマンドリスト（t_cmd）の解放
void	free_cmd(t_cmd **lst)
{
	t_lexer	*redirection_tmp;

	// t_cmd *tmp;
	if (!*lst) // リストがNULLの場合
		return ;
	// while (*lst)
	// 	tmp = (*lst)->next;
	redirection_tmp = (*lst)->redirections;
	close_heredocs(redirection_tmp);
	free_lexer(&redirection_tmp);
	if ((*lst)->str)
		free_arr((*lst)->str);
	// if ((*lst)->hd_file_name) // TODO
	// {
	// 	free((*lst)->hd_file_name);
	// 	free(*lst); // 構造体全体を解放
	// 	*lst = tmp; // 次の構造体へ
	// }
	*lst = NULL;
}

int	count_args(t_lexer *lexer_list)
{
	t_lexer	*tmp;
	int		i;

	i = 0;
	tmp = lexer_list;
	while (tmp && tmp->token != PIPE)
	{
		if (tmp->i >= 0)
		{
			i++;
			while (tmp->join_next && tmp->next && tmp->next->token != PIPE)
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (i);
}

t_cmd	*create_cmd(char **str, int num_redirections, t_lexer *redirections)
{
	t_cmd	*new_node;

	new_node = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_node)
		return (0);
	new_node->str = str;
	new_node->builtin = builtin_arr(str[0]);
	// new_node->hd_file_name = NULL;
	new_node->num_redirections = num_redirections;
	new_node->redirections = redirections;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	add_cmd_back(t_cmd **lst, t_cmd *new_cmd)
{
	t_cmd	*tmp;

	tmp = *lst;
	if (*lst == NULL)
	{
		*lst = new_cmd;
		return ;
	}
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new_cmd;
	new_cmd->prev = tmp;
}

// 1つのコマンドを構造化して、t_cmdを生成する
// 必要なヘッダーがあれば追加してください（libft.hなど）

t_cmd	*init_cmd(t_parser_shell *parser_shell)
{
	int		arg_size;
	int		i;
	t_lexer	*tmp;
	char	**str;
	int		current_index;

	char *temp; // 結合用の一時変数
	i = 0;
	if (remove_redirections(parser_shell) == EXIT_FAILURE)
		return (NULL);
	// ★修正したcount_argsを使用
	arg_size = count_args(parser_shell->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		parser_error(1, parser_shell->lexer_list);
	tmp = parser_shell->lexer_list;
	while (arg_size > 0)
	{
		if (tmp && tmp->str)
		{
			// 1. 最初の部分をコピー
			str[i] = ft_strdup(tmp->str);
			// 保存用: 現在のノードのインデックス
			current_index = tmp->i;
			// ★追加: 結合ループ
			// join_nextが立っていて、次が存在するなら結合し続ける
			while (tmp->join_next && tmp->next)
			{
				tmp = tmp->next; // 次へ進む
				// 文字列を結合 (str[i] + tmp->str)
				temp = ft_strjoin(str[i], tmp->str);
				free(str[i]); // 古い方を解放
				str[i] = temp;
				// 吸収された「次のノード」を削除リストから消す
				// (remove_nodeの実装によっては副作用に注意が必要ですが、
				//  まずはリストから論理的に消せればOK)
				remove_node(&parser_shell->lexer_list, tmp->i);
			}
			// 結合が終わった後、最初の「親玉ノード」を削除
			remove_node(&parser_shell->lexer_list, current_index);
			i++;
		}
		// リストの先頭から再スキャン（remove_nodeでリストが変わるため）
		tmp = parser_shell->lexer_list;
		arg_size--;
	}
	return (create_cmd(str, parser_shell->num_redirections,
			parser_shell->redirections));
}
