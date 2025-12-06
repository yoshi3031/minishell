/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 02:28:38 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/06 15:17:39 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# define MAX_VAR_NAME 256

# include "minishell.h"
# include <stdbool.h> // bool型を使うため
# include <stddef.h>  // size_t を使うため

// 展開時に使う構造体（必要なら）
typedef struct s_expand
{
	const char	*input;
	char		*result;
	size_t		i;
	size_t		j;
	bool		in_single_quote;
	bool		in_double_quote;
}				t_expand;

// 環境変数の値を取得
char			*get_env_value(char **env, const char *key);

// 展開後のサイズを計算
size_t			calc_expanded_length(const char *input, char **env,
					int last_status);

// 実際の展開処理
char			*expand_token(const char *input, char **env, int last_status);
void			expand_all_tokens(char **args, char **env, int last_status);
void			expand_all_redirs(t_lexer *redir, char **env, int last_status);
void			expand_all_cmds(t_cmd *cmds, char **env, int last_status);
void			expand_cmd(t_cmd *cmd, char **env, int last_status);

// 補助関数
size_t			get_env_len(char **env, const char *key);
size_t			count_status_len(int last_status);
size_t			handle_dollar_len(const char *s, size_t *i, char **env,
					int last_status);
void			handle_dollar(t_expand *e, char **env, int last_status);
void			init_expand(t_expand *e, const char *input);
void			handle_exit_status(t_expand *e, int last_status);
void			copy_env_value(t_expand *e, char **env, const char *key);
bool			has_quote(const char *str);

#endif
