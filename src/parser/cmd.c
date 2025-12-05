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

static int	is_joinable(t_tokens token)
{
	return (token == WORD || token == DQUOTE_WORD || token == SQUOTE_WORD
		|| token == ENV_VAR || token == EXIT_STATUS);
}

static void	unite_tokens(t_lexer **lexer_list)
{
	t_lexer	*tmp;
	t_lexer	*next;
	char	*joined;

	if (!lexer_list || !*lexer_list)
		return ;
	tmp = *lexer_list;
	while (tmp)
	{
		// 結合条件:
		// 1. フラグが立っている
		// 2. 次がある
		// 3. 現在のトークンがWORD系である (重要: >file のように結合するのを防ぐ)
		// 4. 次のトークンがパイプや終端ではない
		while (tmp->join_next && tmp->next && is_joinable(tmp->token)
			&& tmp->next->token != PIPE && tmp->next->token != END_OF_INPUT)
		{
			next = tmp->next;
			// 安全策
			if (!tmp->str || !next->str)
				break ;
			joined = ft_strjoin(tmp->str, next->str);
			if (!joined)
				return ; // エラー処理
			free(tmp->str);
			tmp->str = joined;
			tmp->join_next = next->join_next; // フラグ継承
			// クォート状態の更新
			if (next->token == DQUOTE_WORD || next->token == SQUOTE_WORD)
				tmp->token = DQUOTE_WORD;
			// ノード削除処理 (手動で安全に行う)
			tmp->next = next->next;
			if (next->next)
				next->next->prev = tmp;
			if (next->str)
				free(next->str);
			free(next);
		}
		tmp = tmp->next;
	}
}

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
	t_cmd	*tmp;
	t_cmd	*crnt;
	if (!lst || !*lst)
		return ;
	crnt = *lst;
	while(crnt)
	{
		tmp = crnt->next;
		if (crnt->redirections)
		{
			close_heredocs(crnt->redirections);
			free_lexer(&crnt->redirections);
		}
		if (crnt->str)
			free_arr(crnt->str);
		free(crnt);
		crnt = tmp;
	}
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

	unite_tokens(&parser_shell->lexer_list);
	if (remove_redirections(parser_shell) == EXIT_FAILURE)
		return (NULL);
	// ★修正したcount_argsを使用
	arg_size = count_args(parser_shell->lexer_list);
	str = ft_calloc(arg_size + 1, sizeof(char *));
	if (!str)
		ft_error(1);
	tmp = parser_shell->lexer_list;
	i = 0;
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
