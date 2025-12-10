/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yotakagi <yotakagi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 02:28:38 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/10 13:34:06 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# define MAX_VAR_NAME 256

# include "minishell.h"
# include <stdbool.h>
# include <stddef.h>

typedef struct s_expand
{
	const char	*input;
	char		*result;
	size_t		i;
	size_t		j;
	bool		in_single_quote;
	bool		in_double_quote;
}				t_expand;

char			*get_env_value(char **env, const char *key);

size_t			calc_expanded_length(const char *input, char **env,
					int last_status);

char			*expand_token(const char *input, char **env, int last_status);
void			expand_all_tokens(char **args, char **env, int last_status);
void			expand_all_redirs(t_lexer *redir, char **env, int last_status);
void			expand_all_cmds(t_cmd *cmds, char **env, int last_status);
void			expand_cmd(t_cmd *cmd, char **env, int last_status);

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
