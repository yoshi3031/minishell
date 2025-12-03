/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 08:59:59 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/03 02:33:28 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	extract_redirection(t_lexer *tmp, t_parser_shell *parser_shell)
{
	t_lexer	*node;
	int		index_1;
	int		index_2;
	bool	expand;

	//新しいノードの作成。後で元のトークンが削除されるため、保存しておく
	//レダイレクト先のファイル名、tokenはレダイレクト記号を引き継ぐ
	node = create_node(ft_strdup(tmp->next->str), tmp->token);
	if (!node)
		parser_error(1, parser_shell->lexer_list);
	if (tmp->token == HEREDOC)
	{
		//to do クォートありかなしか
		expand = (tmp->next->token == WORD);
		node->heredoc_fd = read_heredoc(tmp->next->str, expand, parser_shell->shell->env, parser_shell->shell->error_num);
		//エラーハンドリング
		if (node->heredoc_fd == -1)//&& g_signal == 1
		{
			//free(node)etc,,,
			free(node->str);
			free(node);
			return(EXIT_FAILURE);//呼び出し元で中断処理
		}
	}
	//そのノードをリストに追加
	add_node_back(&parser_shell->redirections, node);
	index_1 = tmp->i;       //リダイレクト
	index_2 = tmp->next->i; //リダイレクトの引数
	//元のlexer_listからリダイレクトと引数を削除
	remove_node(&parser_shell->lexer_list, index_1);
	remove_node(&parser_shell->lexer_list, index_2);
	//リダイレクトの数をカウント
	parser_shell->num_redirections++;
	return (EXIT_SUCCESS);
}

int	remove_redirections(t_parser_shell *parser_shell)
{
	t_lexer	*tmp;

	tmp = parser_shell->lexer_list;
	//未分類の文字の場合スキップ
	while (tmp && !(tmp->token >= REDIR_OUT && tmp->token <= HEREDOC))
		tmp = tmp->next;
	// 文字列の終わり、もしくはパイプが来たら終了
	if (!tmp || tmp->token == PIPE)
		return (EXIT_SUCCESS);
	// エラー：ファイル名がない
	if (!tmp->next || tmp->next->token == END_OF_INPUT)
		parser_error(0, parser_shell->lexer_list);
	// エラー：リダイレクト連続
	if (tmp->next->token >= PIPE && tmp->next->token <= HEREDOC)
		parser_double_token_error(parser_shell->lexer_list, tmp->next->token);
	// リダイレクトがあったらリダイレクションリストに追加し、リダイレクトとその後の文字列をlexerから削除
	if (tmp->token >= REDIR_OUT && tmp->token <= HEREDOC)
	{
		if (extract_redirection(tmp, parser_shell) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	// 再起呼び出し
	if(remove_redirections(parser_shell) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
