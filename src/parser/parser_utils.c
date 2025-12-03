/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 10:04:52 by nhara             #+#    #+#             */
/*   Updated: 2025/12/03 12:53:41 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//このファイルにはparserで使う小さな関数を入れる

#include "minishell.h"

// lexerの全ノードを解放
void	free_lexer(t_lexer **lst)
{
	t_lexer	*tmp;

	// リストが存在しない場合
	if (!*lst)
		return ;
	while (*lst)
	{
		// 次のリストを保管
		tmp = (*lst)->next;
		if ((*lst)->str) // リストの中に文字列が存在する場合解法
			free((*lst)->str);
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

// パイプの数を数える
void	count_pipes(t_lexer *lexer_list, t_shell *shell)
{
	t_lexer	*tmp;

	tmp = lexer_list;
	shell->pipes = 0;
	while (tmp)
	{
		if (tmp->token == PIPE)
			shell->pipes++;
		tmp = tmp->next;
	}
}

t_parser_shell	init_parser_shell(t_lexer *lexer_list, t_shell *shell)
{
	t_parser_shell	parser_shell;

	parser_shell.lexer_list = lexer_list;
	parser_shell.redirections = NULL;
	parser_shell.num_redirections = 0;
	parser_shell.shell = shell;
	return (parser_shell);
}
