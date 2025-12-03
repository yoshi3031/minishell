/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 18:56:10 by nagisa            #+#    #+#             */
/*   Updated: 2025/12/03 02:32:40 by ayamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "minishell.h"
#include <stdbool.h>
#include <stddef.h>

// 現在のコマンドブロック解析のための構造体。最終的に整理された状態でt_cmdに保存
typedef struct s_parser_shell
{
	t_lexer *lexer_list;
	t_lexer *redirections; // リダイレクトのリスト
	int num_redirections;  // リダイレクトの数
	struct s_shell *shell;
} t_parser_shell;

// 全体のshellの状態
typedef struct s_shell
{
	char *args;	  // ユーザーが入力したコマンドライン全体をそのまま保持
	char **paths; // PATH 環境変数から分割した「パスリスト（文字列配列）」を保持
	char **env;
	int pipes; // パイプの数
	int *pid;
	t_lexer *lexer_list;
	struct s_cmd *cmd; //TODO なぜstructを作る
	// boolは真偽値を表すデータ型 で、true（真）または false（偽）のいずれかを取る
	bool reset;
	bool heredoc;//使用しているか確認中
	int error_num;
	// int stop_heredoc;
	// int in_cmd;
	// int in_heredoc;
} t_shell;

// 各コマンド（echo hello、grep hello、wc -l など）をリンクリスト形式で保存
typedef struct s_cmd
{
	char **str; // コマンドと引数を2D配列で保存
	// char *hd_file_name;
	//builtinの関数へのポインタを持つ。builtin以外だとNULLを持つ。
	int (*builtin)(t_shell *, struct s_cmd *);
	int num_redirections;
	t_lexer *redirections; // リダイレクトの保存
	struct s_cmd *next;
	struct s_cmd *prev;
} t_cmd;

int parser(t_shell *shell);
// cmd
void free_cmd(t_cmd **lst);
t_cmd *init_cmd(t_parser_shell *parser_shell);
void add_cmd_back(t_cmd **lst, t_cmd *new_cmd);

// parser_utils
void count_pipes(t_lexer *lexer_list, t_shell *shell);
t_parser_shell init_parser_shell(t_lexer *lexer_list, t_shell *shell);

// parser_error
int parser_double_token_error(t_lexer *lexer_list, t_tokens token);
int pipe_errors(t_shell *shell, t_tokens token);
void parser_error(int error, t_lexer *lexer_list);
int ft_error(int error);

// node
void free_lexer(t_lexer **lst);
void remove_node(t_lexer **lst, int target);
t_lexer *create_node(char *str, int token);
void add_node_back(t_lexer **lst, t_lexer *new_node);

// libft
void ft_putstr_fd(char *s, int fd);
void *ft_calloc(size_t nmemb, size_t size);
char *ft_strdup(const char *s);
size_t ft_strlen(const char *s);

// shell
int reset_shell(t_shell *shell);
void free_arr(char **arr);
void init_shell(t_shell *shell);
int loop(t_shell *shell);

// redirections
int remove_redirections(t_parser_shell *parser_shell);

// quotes
int validate_quotes(char *args);

// main
void print_parsed_commands(t_shell *shell);
#endif
