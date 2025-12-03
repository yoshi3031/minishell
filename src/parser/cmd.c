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
	// パイプが出てくるまで
	while (tmp && tmp->token != PIPE)
	{
		// 削除済みトークンが残っている可能性があるため、有効なトークンのみを数える
		//これ何の意味がある？
		if (tmp->i >= 0)
			i++;
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
t_cmd	*init_cmd(t_parser_shell *parser_shell)
{
	int		arg_size;
	int		i;
	t_lexer	*tmp;

	char **str; // ここに格納する
	i = 0;
	// リダイレクトを取り除きredirectionsリストに格納
	if (remove_redirections(parser_shell) == EXIT_FAILURE)
		return (NULL);
	// 1つのコマンドブロックの引数の数を数える
	arg_size = count_args(parser_shell->lexer_list);
	// 二次元配列の作成
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		parser_error(1, parser_shell->lexer_list);
	tmp = parser_shell->lexer_list;
	while (0 < arg_size)
	{
		if (tmp->str) // 文字列が入っていれば
		{
			// コマンドを格納
			str[i++] = ft_strdup(tmp->str);
			// 今処理した文字列を削除
			remove_node(&parser_shell->lexer_list, tmp->i);
			// 先頭が変わるため更新する
			tmp = parser_shell->lexer_list;
		}
		arg_size--;
	}
	return (create_cmd(str, parser_shell->num_redirections,
			parser_shell->redirections));
}
