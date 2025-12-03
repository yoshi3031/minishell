/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayamamot <ayamamot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 14:14:42 by yotakagi          #+#    #+#             */
/*   Updated: 2025/12/03 12:41:07 by yotakagi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include "parser.h"

typedef int		(*t_builtin_fn)(t_shell *shell, t_cmd *cmd);

int				minishell_echo(t_shell *shell, t_cmd *cmd);
int				minishell_cd(t_shell *shell, t_cmd *cmd);
int				minishell_pwd(t_shell *shell, t_cmd *cmd);
int				minishell_export(t_shell *shell, t_cmd *cmd);
int				minishell_unset(t_shell *shell, t_cmd *cmd);
int				minishell_env(t_shell *shell, t_cmd *cmd);
int				minishell_exit(t_shell *shell, t_cmd *cmd);
int				is_numeric(const char *str);
void			exit_numeric_error(const char *arg);
void			exit_too_many_args(t_shell *shell);
int				is_valid_key(const char *key);
void			export_error(const char *key);
void			remove_env_entry(char ***env, const char *key);
int				count_2d_arr(char **arr);
void			free_2d_arr(char **arr);
char			*get_key(const char *arg);
char			*get_val(const char *arg);
void			print_env_sorted(char **env);
t_builtin_fn	builtin_arr(char *str);
void			update_or_add_env(t_shell *shell, char *key, char *val);
int				find_env_entry(char **env, char *key);
char			*my_getenv(char **env, char *key);

#endif
